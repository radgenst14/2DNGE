#include "ScriptEngine.h"
#include "ScriptingConfig.h"
#include <SDL.h>
#include <fstream>
#include <sstream>
#include <string>

int ScriptEngine::sRefCount = 0;

ScriptEngine::ScriptEngine()
    : mInitialized(false)
{
    try
    {
        if (sRefCount == 0)
        {
            // Tell Python where its standard library lives
            std::string narrow(PYTHON_HOME);
            static std::wstring pythonHome(narrow.begin(), narrow.end());
            Py_SetPythonHome(pythonHome.c_str());

            py::initialize_interpreter();
            SDL_Log("Python interpreter initialized");
        }
        ++sRefCount;
        mInitialized = true;
    }
    catch (const py::error_already_set &e)
    {
        SDL_Log("Failed to initialize Python: %s", e.what());
    }
}

ScriptEngine::~ScriptEngine()
{
    if (mInitialized)
    {
        // Release Python objects before potentially finalizing the interpreter
        mGlobals = py::object();

        --sRefCount;
        if (sRefCount == 0)
        {
            py::finalize_interpreter();
            SDL_Log("Python interpreter shut down");
        }
    }
}

py::object ScriptEngine::execute(const std::string &code)
{
    return py::eval(code);
}

bool ScriptEngine::loadScript(const std::string &path)
{
    try
    {
        // Create a fresh namespace for the script
        mGlobals = py::dict();

        // Add Python builtins so the script can use print(), range(), etc.
        mGlobals["__builtins__"] = py::module_::import("builtins");

        // Read the script file and execute it into our namespace
        std::ifstream file(path);
        if (!file.is_open())
        {
            SDL_Log("Cannot open script file '%s'", path.c_str());
            return false;
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        py::exec(ss.str(), mGlobals);
        return true;
    }
    catch (const py::error_already_set &e)
    {
        SDL_Log("Failed to load script '%s': %s", path.c_str(), e.what());
        return false;
    }
}