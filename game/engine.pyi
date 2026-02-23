"""Auto-generated type stubs for the 2DNGE engine module.

DO NOT EDIT -- regenerate with: python tools/generate_stubs.py
"""

from typing import Tuple

# -- ECS ----------------------------------------------------

def create_entity() -> int:
    """Create a new entity and return its unique ID."""
    ...

def add_transform(entity: int, x: float, y: float) -> None:
    """Add a Transform component to an entity."""
    ...

def add_rigidbody(entity: int, vx: float, vy: float, mass: float) -> None:
    """Add a RigidBody component to an entity."""
    ...

def get_position(entity: int) -> Tuple[float, float]:
    """Get the position of an entity's Transform component."""
    ...

# -- Physics ------------------------------------------------

def physics_update(dt: float) -> None:
    """Update the physics simulation by a given time step (dt). This will move all physics-enabled entities according to their velocities and handle collisions."""
    ...

# -- Render -------------------------------------------------

def render() -> None:
    """Render the current frame, including all sprites and other renderable entities."""
    ...

# -- Input --------------------------------------------------

def is_key_down(scancode: int) -> bool:
    """Check if a key is currently being held down. Returns True if the key corresponding to the given scancode is pressed, False otherwise."""
    ...

def is_key_pressed(scancode: int) -> bool:
    """Check if a key was pressed this frame. Returns True if the key corresponding to the given scancode was pressed this frame, False otherwise."""
    ...

def is_key_released(scancode: int) -> bool:
    """Check if a key was released this frame. Returns True if the key corresponding to the given scancode was released this frame, False otherwise."""
    ...

KEY_A: int
KEY_B: int
KEY_C: int
KEY_D: int
KEY_E: int
KEY_F: int
KEY_G: int
KEY_H: int
KEY_I: int
KEY_J: int
KEY_K: int
KEY_L: int
KEY_M: int
KEY_N: int
KEY_O: int
KEY_P: int
KEY_Q: int
KEY_R: int
KEY_S: int
KEY_T: int
KEY_U: int
KEY_V: int
KEY_W: int
KEY_X: int
KEY_Y: int
KEY_Z: int
KEY_0: int
KEY_1: int
KEY_2: int
KEY_3: int
KEY_4: int
KEY_5: int
KEY_6: int
KEY_7: int
KEY_8: int
KEY_9: int
KEY_SPACE: int
KEY_RETURN: int
KEY_ESCAPE: int
KEY_LSHIFT: int
KEY_RSHIFT: int
KEY_LCTRL: int
KEY_RCTRL: int
KEY_TAB: int
KEY_UP: int
KEY_DOWN: int
KEY_LEFT: int
KEY_RIGHT: int
