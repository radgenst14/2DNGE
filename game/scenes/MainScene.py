import engine

entity1 = None

def init():
    global entity1

    # Load the sprite texture from the .aseprite file
    sprite_path = engine.PROJECT_ROOT + "/game/sprites/sample-001.aseprite"
    engine.load_aseprite("player", sprite_path)

    # Create an entity with transform, physics, and sprite
    entity1 = engine.create_entity()
    engine.add_transform(entity1, 100.0, 100.0)
    engine.add_rigidbody(entity1, 50.0, 0.0, 1.0)
    engine.add_sprite(entity1, "player")

def update(dt):
    engine.physics_update(dt)

def input():
    pass

def render():
    engine.render()

def cleanup():
    pass
