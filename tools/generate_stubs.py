#!/usr/bin/env python3
"""Generate engine.pyi type stubs from C++ pybind11 binding sources."""

import argparse
import glob
import os
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# ── C++ to Python type mapping ───────────────────────

CPP_TO_PYTHON: Dict[str, str] = {
    "float": "float",
    "double": "float",
    "int": "int",
    "unsigned int": "int",
    "uint32_t": "int",
    "int32_t": "int",
    "size_t": "int",
    "bool": "bool",
    "void": "None",
    "EntityID": "int",
    "py::tuple": "Tuple[float, float]",
    "pybind11::tuple": "Tuple[float, float]",
    "std::string": "str",
    "py::str": "str",
}

# For m.attr() type wrappers
ATTR_TYPE_MAP: Dict[str, str] = {
    "py::int_": "int",
    "py::float_": "float",
    "py::bool_": "bool",
    "py::str": "str",
}

# Return type overrides for lambdas with `return` but no explicit `-> type`
RETURN_TYPE_OVERRIDES: Dict[str, str] = {
    "create_entity": "int",
    "is_key_down": "bool",
    "is_key_pressed": "bool",
    "is_key_released": "bool",
}


# ── Data classes ─────────────────────────────────────

@dataclass
class FunctionDef:
    name: str
    params: List[Tuple[str, str]]  # [(python_type, param_name), ...]
    return_type: str
    docstring: Optional[str]


@dataclass
class ConstantDef:
    name: str
    python_type: str


@dataclass
class BindingGroup:
    label: str
    functions: List[FunctionDef] = field(default_factory=list)
    constants: List[ConstantDef] = field(default_factory=list)


# ── Parsing helpers ──────────────────────────────────

def strip_cpp_comments(source: str) -> str:
    """Remove C and C++ style comments from source."""
    # Remove block comments
    source = re.sub(r"/\*.*?\*/", "", source, flags=re.DOTALL)
    # Remove line comments
    source = re.sub(r"//[^\n]*", "", source)
    return source


def extract_m_def_blocks(source: str) -> List[str]:
    """Extract complete m.def(...) call strings using paren-depth tracking."""
    blocks = []
    i = 0
    while True:
        idx = source.find("m.def(", i)
        if idx == -1:
            break
        depth = 1
        pos = idx + len("m.def(")
        in_string = False
        string_char = None
        while pos < len(source) and depth > 0:
            c = source[pos]
            if in_string:
                if c == "\\" and pos + 1 < len(source):
                    pos += 1  # skip escaped char
                elif c == string_char:
                    in_string = False
            else:
                if c in ('"', "'"):
                    in_string = True
                    string_char = c
                elif c == "(":
                    depth += 1
                elif c == ")":
                    depth -= 1
            pos += 1
        blocks.append(source[idx:pos])
        i = pos
    return blocks


def extract_m_attr_entries(source: str) -> List[ConstantDef]:
    """Extract m.attr("NAME") = py::type_(...) constants."""
    constants = []
    for match in re.finditer(
        r'm\.attr\(\s*"([^"]+)"\s*\)\s*=\s*(py::\w+)\s*\(', source
    ):
        name = match.group(1)
        type_wrapper = match.group(2)
        python_type = ATTR_TYPE_MAP.get(type_wrapper, "int")
        constants.append(ConstantDef(name=name, python_type=python_type))
    return constants


def map_cpp_type(cpp_type: str) -> str:
    """Map a C++ type string to a Python type string."""
    cpp_type = cpp_type.strip()
    # Remove const, &, *
    cleaned = cpp_type.replace("const ", "").replace("&", "").replace("*", "").strip()

    # Check direct mapping
    if cleaned in CPP_TO_PYTHON:
        return CPP_TO_PYTHON[cleaned]

    # Handle std::tuple<T1, T2, ...>
    tuple_match = re.match(r"std::tuple<(.+)>", cleaned)
    if tuple_match:
        inner = tuple_match.group(1)
        inner_types = [map_cpp_type(t.strip()) for t in inner.split(",")]
        return "Tuple[{}]".format(", ".join(inner_types))

    return cleaned


def parse_params(params_str: str) -> List[Tuple[str, str]]:
    """Parse a C++ lambda parameter list into [(python_type, name), ...]."""
    params_str = params_str.strip()
    if not params_str:
        return []

    params = []
    for param in params_str.split(","):
        param = param.strip()
        if not param:
            continue
        # Split "Type name" from the right (handles multi-word types like "unsigned int")
        parts = param.rsplit(None, 1)
        if len(parts) == 2:
            cpp_type, name = parts
            params.append((map_cpp_type(cpp_type), name))
    return params


def parse_m_def_block(block: str) -> Optional[FunctionDef]:
    """Parse a single m.def(...) block into a FunctionDef."""
    # Extract function name
    name_match = re.search(r'm\.def\(\s*"([^"]+)"', block)
    if not name_match:
        return None
    func_name = name_match.group(1)

    # Extract lambda parameter list: [...](params)
    lambda_match = re.search(r"\[\s*[^\]]*\]\s*\(([^)]*)\)", block)
    params_str = lambda_match.group(1) if lambda_match else ""
    params = parse_params(params_str)

    # Extract explicit return type: (...) -> ReturnType {
    return_match = re.search(
        r"\]\s*\([^)]*\)\s*->\s*([\w:]+(?:<[^>]+>)?)", block
    )
    explicit_return = return_match.group(1) if return_match else None

    # Determine return type
    if explicit_return:
        return_type = map_cpp_type(explicit_return)
    elif func_name in RETURN_TYPE_OVERRIDES:
        return_type = RETURN_TYPE_OVERRIDES[func_name]
    elif "return " in block:
        # Has a return statement but no explicit type and no override — warn
        print(
            "Warning: {} has return but no explicit type; defaulting to None".format(func_name),
            file=sys.stderr,
        )
        return_type = "None"
    else:
        return_type = "None"

    # Extract docstring — last quoted string before the closing paren
    # Match the final string argument: , "docstring")
    docstring = None
    doc_matches = re.findall(r',\s*"((?:[^"\\]|\\.)*)"[^"]*$', block)
    if doc_matches:
        docstring = doc_matches[-1]

    return FunctionDef(
        name=func_name,
        params=params,
        return_type=return_type,
        docstring=docstring,
    )


# ── File-level parsing ───────────────────────────────

def derive_group_label(filepath: str) -> str:
    """Derive a section label from a binding filename (e.g. ECSBindings.cpp -> ECS)."""
    basename = os.path.basename(filepath)
    label = basename.replace("Bindings.cpp", "").replace("Bindings.h", "")
    return label


def parse_binding_file(filepath: str) -> BindingGroup:
    """Parse a single C++ binding file and return its functions and constants."""
    with open(filepath, "r", encoding="utf-8") as f:
        source = f.read()

    source = strip_cpp_comments(source)
    label = derive_group_label(filepath)

    functions = []
    for block in extract_m_def_blocks(source):
        func = parse_m_def_block(block)
        if func:
            functions.append(func)

    constants = extract_m_attr_entries(source)

    return BindingGroup(label=label, functions=functions, constants=constants)


# ── .pyi generation ──────────────────────────────────

def generate_pyi(groups: List[BindingGroup]) -> str:
    """Generate the full .pyi file content."""
    lines = [
        '"""Auto-generated type stubs for the 2DNGE engine module.',
        "",
        "DO NOT EDIT -- regenerate with: python tools/generate_stubs.py",
        '"""',
        "",
    ]

    # Collect needed typing imports
    needs_tuple = any(
        "Tuple[" in f.return_type or any("Tuple[" in p[0] for p in f.params)
        for g in groups
        for f in g.functions
    )
    if needs_tuple:
        lines.append("from typing import Tuple")
        lines.append("")

    for group in groups:
        if not group.functions and not group.constants:
            continue

        lines.append("# -- {} {}".format(group.label, "-" * max(1, 55 - len(group.label))))
        lines.append("")

        for func in group.functions:
            param_strs = ["{}: {}".format(name, ptype) for ptype, name in func.params]
            sig = "def {}({}) -> {}:".format(func.name, ", ".join(param_strs), func.return_type)
            lines.append(sig)
            if func.docstring:
                lines.append('    """{}"""'.format(func.docstring))
            lines.append("    ...")
            lines.append("")

        for const in group.constants:
            lines.append("{}: {}".format(const.name, const.python_type))

        if group.constants:
            lines.append("")

    return "\n".join(lines)


# ── CLI ──────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="Generate engine.pyi from C++ pybind11 binding sources."
    )
    parser.add_argument(
        "files",
        nargs="*",
        help="C++ binding source files. If omitted, auto-discovers engine/scripting/bindings/*Bindings.cpp",
    )
    parser.add_argument(
        "-o", "--output",
        default=None,
        help="Output .pyi path (default: game/engine.pyi relative to project root)",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Check mode: exit with error if existing .pyi differs from generated",
    )
    args = parser.parse_args()

    # Determine project root (script lives in tools/)
    script_dir = Path(__file__).resolve().parent
    project_root = script_dir.parent

    # Determine binding files
    if args.files:
        binding_files = [str(Path(f).resolve()) for f in args.files]
    else:
        pattern = str(project_root / "engine" / "scripting" / "bindings" / "*Bindings.cpp")
        binding_files = sorted(glob.glob(pattern))

    if not binding_files:
        print("Error: no binding files found.", file=sys.stderr)
        sys.exit(1)

    # Parse all binding files
    groups = []
    for filepath in binding_files:
        group = parse_binding_file(filepath)
        groups.append(group)

    # Generate .pyi content
    content = generate_pyi(groups)

    # Determine output path
    if args.output:
        output_path = Path(args.output).resolve()
    else:
        output_path = project_root / "game" / "engine.pyi"

    if args.check:
        if output_path.exists():
            existing = output_path.read_text(encoding="utf-8")
            if existing == content:
                print("engine.pyi is up to date.")
                sys.exit(0)
            else:
                print("engine.pyi is out of date! Regenerate with:", file=sys.stderr)
                print("  python tools/generate_stubs.py", file=sys.stderr)
                sys.exit(1)
        else:
            print("engine.pyi does not exist!", file=sys.stderr)
            sys.exit(1)

    # Write output
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")
    print("Generated {}".format(output_path))


if __name__ == "__main__":
    main()
