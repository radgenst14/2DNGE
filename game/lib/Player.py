# game/player.py
import engine
from game.lib.GameObject import GameObject

class Player(GameObject):
    def __init__(self, x, y):
        super().__init__(x, y)
        self.add_sprite("player") \
            .add_rigidbody(mass=1.0) \
            .add_box_collider(20.0, 28.0, 0.0, 2.0)
        self.speed = 200.0

    def update(self, dt):
        vx = 0.0
        if engine.is_key_down(engine.KEY_LEFT): 
            vx = -self.speed
        if engine.is_key_down(engine.KEY_RIGHT): 
            vx = self.speed
        engine.set_velocity(self.id, vx, 0.0)