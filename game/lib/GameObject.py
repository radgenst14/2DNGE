# game/gameobject.py
import engine

class GameObject:
    def __init__(self, x=0.0, y=0.0):
        self.id = engine.create_entity()
        engine.add_transform(self.id, x, y)

    def get_position(self):
        return engine.get_position(self.id)

    def set_position(self, x, y):
        engine.set_position(self.id, x, y)

    def add_sprite(self, texture_id, width=0, height=0):
        engine.add_sprite(self.id, texture_id, width, height)
        return self

    def add_rigidbody(self, vx=0.0, vy=0.0, mass=1.0):
        engine.add_rigidbody(self.id, vx, vy, mass)
        return self

    def add_box_collider(self, width, height, ox=0.0, oy=0.0):
        engine.add_collider_box(self.id, width, height, ox, oy)
        return self

    def add_circle_collider(self, radius, ox=0.0, oy=0.0):
        engine.add_collider_circle(self.id, radius, ox, oy)
        return self

    def play_animation(self, tag_name):
        engine.play_animation(self.id, tag_name)

    def update(self, dt):
        """Override in subclasses for per-entity behavior."""
        pass