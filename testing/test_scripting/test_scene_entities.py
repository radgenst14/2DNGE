import engine

entities = []

def init():
    for i in range(5):
        e = engine.create_entity()
        engine.add_transform(e, float(i * 10), float(i * 20))
        engine.add_rigidbody(e, 1.0, 0.0, 1.0)
        entities.append(e)

def update(dt):
    for e in entities:
        pos = engine.get_position(e)

def cleanup():
    pass
