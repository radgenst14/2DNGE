import engine

initialized = False
updated = False
cleaned_up = False
update_dt = 0.0

def init():
    global initialized
    initialized = True
    e = engine.create_entity()
    engine.add_transform(e, 10.0, 20.0)

def update(dt):
    global updated, update_dt
    updated = True
    update_dt = dt

def cleanup():
    global cleaned_up
    cleaned_up = True
