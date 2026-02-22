import engine

entity1 = None

def init():
    global entity1
    entity1 = engine.create_entity()
    engine.add_transform(entity1, 100.0, 100.0)
    engine.add_rigidbody(entity1, 50.0, 0.0, 1.0)

def update(dt):
    engine.physics_update(dt)

def cleanup():
    pass
