import engine

entities = []
init_called = False
input_called = False
update_called = False
render_called = False
cleanup_called = False

def init():
    global init_called
    init_called = True
    for i in range(3):
        e = engine.create_entity()
        engine.add_transform(e, float(i * 10), float(i * 20))
        entities.append(e)

def input():
    global input_called
    input_called = True

def update(dt):
    global update_called
    update_called = True

def render():
    global render_called
    render_called = True
    engine.render()

def cleanup():
    global cleanup_called
    cleanup_called = True
