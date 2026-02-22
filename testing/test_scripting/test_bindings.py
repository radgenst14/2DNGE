import engine

def setup():
    e = engine.create_entity() # pyright: ignore[reportAttributeAccessIssue]
    engine.add_transform(e, 50.0, 75.0) # type: ignore
    engine.add_rigidbody(e, 1.0, 0.0, 1.0) # type: ignore