import engine
from typing import Optional
from game.lib.GameObject import GameObject

SPRITES_FOLDER = "/game/sprites"

# --- Game constants ---
SCREEN_W = 400
SCREEN_H = 300
PADDLE_W = 64
PADDLE_H = 12
PADDLE_Y = 120.0        # near bottom of view
PADDLE_SPEED = 300.0

BALL_SIZE = 8
BALL_SPEED = 180.0

BRICK_W = 32
BRICK_H = 12
BRICK_COLS = 10
BRICK_ROWS = 5
BRICK_PADDING = 4
BRICK_OFFSET_X = -(BRICK_COLS * (BRICK_W + BRICK_PADDING)) / 2 + BRICK_PADDING / 2
BRICK_OFFSET_Y = -100.0  # top area

# --- Game state ---
paddle: Optional[GameObject] = None
ball: Optional[GameObject] = None
bricks: list[GameObject] = []
brick_alive: list[bool] = []

ball_vx: float = 0.0
ball_vy: float = 0.0
ball_launched: bool = False
launch_cooldown: float = 0.0
score: int = 0
lives: int = 3
game_over: bool = False
game_won: bool = False


def init():
    global paddle, ball, bricks, brick_alive
    global ball_vx, ball_vy, ball_launched, score, lives, game_over, game_won

    # Load assets — you'll create these sprites to match:
    #   "paddle"    – 64x12 horizontal paddle
    #   "ball"      – 8x8 ball
    #   "brick_red" – 32x12 red brick
    #   "brick_orange" – 32x12 orange brick
    #   "brick_green"  – 32x12 green brick
    engine.load_aseprite("paddle", engine.PROJECT_ROOT + SPRITES_FOLDER + "/Paddle.aseprite")
    engine.load_aseprite("ball", engine.PROJECT_ROOT + SPRITES_FOLDER + "/Ball.aseprite")
    engine.load_aseprite("brick_red", engine.PROJECT_ROOT + SPRITES_FOLDER + "/BrickRed.aseprite")
    engine.load_aseprite("brick_orange", engine.PROJECT_ROOT + SPRITES_FOLDER + "/BrickOrange.aseprite")
    engine.load_aseprite("brick_green", engine.PROJECT_ROOT + SPRITES_FOLDER + "/BrickGreen.aseprite")

    # Reset state
    ball_vx = 0.0
    ball_vy = 0.0
    ball_launched = False
    score = 0
    lives = 3
    game_over = False
    game_won = False

    # Paddle
    paddle = GameObject(0.0, PADDLE_Y)
    paddle.add_sprite("paddle", PADDLE_W, PADDLE_H)
    paddle.add_box_collider(PADDLE_W, PADDLE_H)

    # Ball — starts sitting on the paddle
    ball = GameObject(0.0, PADDLE_Y - PADDLE_H)
    ball.add_sprite("ball", BALL_SIZE, BALL_SIZE)
    ball.add_box_collider(BALL_SIZE, BALL_SIZE)

    # Bricks
    bricks = []
    brick_alive = []
    row_textures = ["brick_red", "brick_red", "brick_orange", "brick_orange", "brick_green"]

    for row in range(BRICK_ROWS):
        for col in range(BRICK_COLS):
            bx = BRICK_OFFSET_X + col * (BRICK_W + BRICK_PADDING)
            by = BRICK_OFFSET_Y + row * (BRICK_H + BRICK_PADDING)
            brick = GameObject(bx, by)
            brick.add_sprite(row_textures[row], BRICK_W, BRICK_H)
            brick.add_box_collider(BRICK_W, BRICK_H)
            bricks.append(brick)
            brick_alive.append(True)

    # Camera centered at origin, fixed
    engine.set_camera_position(0.0, 0.0)
    engine.set_camera_zoom(1.0)
    engine.draw_colliders(False)


def input():
    pass


def update(dt):
    global ball_vx, ball_vy, ball_launched, launch_cooldown, score, lives, game_over, game_won

    if launch_cooldown > 0:
        launch_cooldown -= dt

    if game_over or game_won:
        # Press R to restart
        if engine.is_key_pressed(engine.KEY_R):
            _restart()
        return

    assert paddle is not None
    assert ball is not None

    # --- Paddle movement ---
    pvx = 0.0
    if engine.is_key_down(engine.KEY_LEFT) or engine.is_key_down(engine.KEY_A):
        pvx = -PADDLE_SPEED
    if engine.is_key_down(engine.KEY_RIGHT) or engine.is_key_down(engine.KEY_D):
        pvx = PADDLE_SPEED

    px, py = paddle.get_position()
    new_px = px + pvx * dt
    half_pw = PADDLE_W / 2
    boundary_x = SCREEN_W / 2 - half_pw
    if new_px < -boundary_x:
        new_px = -boundary_x
    if new_px > boundary_x:
        new_px = boundary_x
    paddle.set_position(new_px, py)

    # --- Launch ball ---
    if not ball_launched:
        # Ball sits on paddle
        ball.set_position(new_px, PADDLE_Y - PADDLE_H)
        if engine.is_key_pressed(engine.KEY_SPACE) and launch_cooldown <= 0:
            ball_launched = True
            ball_vx = BALL_SPEED * 0.7
            ball_vy = -BALL_SPEED
        return

    # --- Move ball manually (no rigidbody, we handle bouncing ourselves) ---
    bx, by = ball.get_position()
    bx += ball_vx * dt
    by += ball_vy * dt

    # Wall bounces (left/right)
    half_screen = SCREEN_W / 2
    half_ball = BALL_SIZE / 2
    if bx - half_ball < -half_screen:
        bx = -half_screen + half_ball
        ball_vx = abs(ball_vx)
    elif bx + half_ball > half_screen:
        bx = half_screen - half_ball
        ball_vx = -abs(ball_vx)

    # Ceiling bounce
    ceiling = -SCREEN_H / 2
    if by - half_ball < ceiling:
        by = ceiling + half_ball
        ball_vy = abs(ball_vy)

    # Floor — lose a life
    floor = SCREEN_H / 2
    if by + half_ball > floor:
        lives -= 1
        if lives <= 0:
            game_over = True
        else:
            _reset_ball()
        return

    # --- Paddle collision ---
    if ball_vy > 0:  # only when falling
        if _box_overlap(bx, by, BALL_SIZE, BALL_SIZE, new_px, PADDLE_Y, PADDLE_W, PADDLE_H):
            by = PADDLE_Y - PADDLE_H / 2 - half_ball
            ball_vy = -abs(ball_vy)
            # Angle based on where ball hits paddle
            hit_offset = (bx - new_px) / (PADDLE_W / 2)  # -1 to 1
            ball_vx = BALL_SPEED * hit_offset * 1.2

    # --- Brick collision ---
    for i, brick in enumerate(bricks):
        if not brick_alive[i]:
            continue
        brx, bry = brick.get_position()
        if _box_overlap(bx, by, BALL_SIZE, BALL_SIZE, brx, bry, BRICK_W, BRICK_H):
            # Destroy brick
            brick_alive[i] = False
            brick.set_position(9999.0, 9999.0)  # move offscreen
            score += 10

            # Bounce — determine which side was hit
            dx = bx - brx
            dy = by - bry
            if abs(dx) / BRICK_W > abs(dy) / BRICK_H:
                ball_vx = -ball_vx
            else:
                ball_vy = -ball_vy
            break  # one collision per frame

    ball.set_position(bx, by)

    # Win check
    if all(not alive for alive in brick_alive):
        game_won = True


def render():
    engine.render()


def cleanup():
    pass


# --- Helpers ---

def _box_overlap(ax, ay, aw, ah, bx, by, bw, bh):
    """AABB overlap check (center-based coordinates)."""
    return (abs(ax - bx) < (aw + bw) / 2 and
            abs(ay - by) < (ah + bh) / 2)


def _reset_ball():
    global ball_vx, ball_vy, ball_launched
    assert paddle is not None
    assert ball is not None
    ball_launched = False
    launch_cooldown = 0.2  # brief cooldown to prevent instant re-launch
    ball_vx = 0.0
    ball_vy = 0.0
    px, py = paddle.get_position()
    ball.set_position(px, PADDLE_Y - PADDLE_H)


def _restart():
    """Full reset — reinitialize the scene."""
    global bricks, brick_alive, score, lives, game_over, game_won
    score = 0
    lives = 3
    game_over = False
    game_won = False
    # Reset bricks to grid positions
    for row in range(BRICK_ROWS):
        for col in range(BRICK_COLS):
            i = row * BRICK_COLS + col
            bx = BRICK_OFFSET_X + col * (BRICK_W + BRICK_PADDING)
            by = BRICK_OFFSET_Y + row * (BRICK_H + BRICK_PADDING)
            bricks[i].set_position(bx, by)
            brick_alive[i] = True
    _reset_ball()
