import engine

SPRITES_FOLDER = "/game/sprites"
entity1 = None

def init():
    global entity1, background

    # --- Background (created FIRST so it renders behind everything) ---
    bg_path = engine.PROJECT_ROOT + SPRITES_FOLDER + "/Debug.aseprite"
    engine.load_aseprite("background", bg_path)

    background = engine.create_entity()
    engine.add_transform(background, 0.0, 0.0)
    engine.add_sprite(background, "background", 400, 300)  # match your window size

    # --- Player (created AFTER, so it renders on top) ---
    sprite_path = engine.PROJECT_ROOT + SPRITES_FOLDER + "/Armored001.aseprite"
    engine.load_aseprite("player", sprite_path)

    entity1 = engine.create_entity()
    engine.add_transform(entity1, -50.0, 0.0)
    engine.add_rigidbody(entity1, 5.0, 0.0, 1.0)
    engine.add_sprite(entity1, "player", 32, 32)

    entity2 = engine.create_entity()
    engine.add_transform(entity2, 50.0, 0.0)
    engine.add_rigidbody(entity2, -5.0, 0.0, 1.0)
    engine.add_sprite(entity2, "player", 32, 32)

    engine.add_collider_box(entity1, 22, 32)
    engine.add_collider_box(entity2, 22, 32)

    engine.draw_colliders(True)

def set_camera_at_sprite(entity):
    x, y = engine.get_position(entity)
    engine.set_camera_position(x, y)
    engine.set_camera_zoom(2.0)

def update(dt):
    engine.physics_update(dt)
    set_camera_at_sprite(entity1)

def input():
    pass

def render():
    engine.render()

def cleanup():
    pass
