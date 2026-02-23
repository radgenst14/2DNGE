import engine

render_count = 0

def init():
    pass

def input():
    pass

def update(dt):
    pass

def render():
    global render_count
    render_count += 1
    # Call engine.render() which goes through RenderManager if available
    engine.render()

def cleanup():
    pass
