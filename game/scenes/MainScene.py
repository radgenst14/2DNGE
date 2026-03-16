import engine
from typing import Optional
from game.lib.GameObject import GameObject
from game.lib.Player import Player

SPRITES_FOLDER = "/game/sprites"

background: Optional[GameObject] = None
player: Optional[Player] = None
entities: list[GameObject] = []


def init():
    global background, player, entities

    engine.load_aseprite("background", engine.PROJECT_ROOT + SPRITES_FOLDER + "/Debug.aseprite")
    engine.load_aseprite("player", engine.PROJECT_ROOT + SPRITES_FOLDER + "/Armored001.aseprite")

    background = GameObject()
    background.add_sprite("background", 400, 300)

    player = Player(0.0, 0.0)
    entities.append(player)

    npc = GameObject(100.0, 0.0).add_rigidbody(mass=1.0).add_box_collider(20.0, 28.0, 0.0, 2.0).add_sprite("player", 64, 64)
    entities.append(npc)

    engine.draw_colliders(True)


def update(dt):
    for entity in entities:
        entity.update(dt)
    engine.physics_update(dt)

    assert player is not None
    px, py = player.get_position()
    engine.set_camera_position(px, py)
    engine.set_camera_zoom(1.0)


def input():
    pass


def render():
    engine.render()


def cleanup():
    pass
