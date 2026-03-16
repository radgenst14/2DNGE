"""Auto-generated type stubs for the 2DNGE engine module.

DO NOT EDIT -- regenerate with: python tools/generate_stubs.py
"""

from typing import Tuple

# -- Asset --------------------------------------------------

def load_texture(id: str, path: str) -> None:
    """Load a texture from a file and store it with a unique ID."""
    ...

def load_aseprite(id: str, path: str) -> None:
    """Load an .aseprite file and store it as a texture with a unique ID."""
    ...

PROJECT_ROOT: str

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

def add_collider_box(entity: int, width: float, height: float, offsetX: float = 0.0, offsetY: float = 0.0) -> None:
    ...

def add_collider_circle(entity: int, radius: float, offsetX: float = 0.0, offsetY: float = 0.0) -> None:
    ...

def get_position(entity: int) -> Tuple[float, float]:
    """Get the position of an entity's Transform component."""
    ...

def set_position(entity: int, x: float, y: float) -> None:
    """Set the position of an entity's Transform component."""
    ...

def get_velocity(entity: int) -> Tuple[float, float]:
    """Get the velocity of an entity's RigidBody component."""
    ...

def set_velocity(entity: int, vx: float, vy: float) -> None:
    """Set the velocity of an entity's RigidBody component."""
    ...

def add_sprite(entity: int, textureId: str, width: int = 0, height: int = 0) -> None:
    ...

def play_animation(entity: int, tagName: str) -> None:
    """Play a named animation tag on the entity's sprite."""
    ...

def pause_animation(entity: int) -> None:
    """Pause the sprite animation."""
    ...

def resume_animation(entity: int) -> None:
    """Resume the sprite animation."""
    ...

def set_animation_looping(entity: int, looping: bool) -> None:
    """Set whether the sprite animation loops."""
    ...

def set_animation_frame(entity: int, frame: int) -> None:
    """Set the current animation frame index."""
    ...

# -- Input --------------------------------------------------

def is_key_down(scancode: int) -> bool:
    """Check if a key is currently being held down."""
    ...

def is_key_pressed(scancode: int) -> bool:
    """Check if a key was pressed this frame."""
    ...

def is_key_released(scancode: int) -> bool:
    """Check if a key was released this frame."""
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

# -- Physics ------------------------------------------------

def physics_update(dt: float) -> None:
    """Update the physics simulation by a given time step (dt). This will move all physics-enabled entities according to their velocities and handle collisions."""
    ...

# -- Render -------------------------------------------------

def render() -> None:
    """Render the current frame, including all sprites and other renderable entities."""
    ...

def set_camera_position(x: float, y: float) -> None:
    """Set the camera position in world coordinates."""
    ...

def get_camera_position() -> Tuple[float, float]:
    """Get the current camera position as (x, y)."""
    ...

def move_camera(dx: float, dy: float) -> None:
    """Move the camera by a delta in world coordinates."""
    ...

def set_camera_zoom(zoom: float) -> None:
    """Set the camera zoom level."""
    ...

def draw_colliders(enabled: bool = True) -> None:
    """Enable or disable debug collider outline drawing."""
    ...

def get_camera_zoom() -> float:
    """Get the current camera zoom level."""
    ...
