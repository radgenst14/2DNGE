import engine

player = None

def init():
    global player
    player = engine.create_entity()
    engine.add_transform(player, 0.0, 0.0)
    engine.add_rigidbody(player, 100.0, 50.0, 1.0)

def update(dt):
    pass

def cleanup():
    pass
