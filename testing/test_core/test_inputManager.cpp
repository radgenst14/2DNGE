#include <gtest/gtest.h>
#include <SDL.h>
#include <cstring>
#include "../../engine/core/InputManager.h"

class InputManagerTest : public ::testing::Test
{
protected:
    Uint8 *mKeyState = nullptr;
    int mNumKeys = 0;

    void SetUp() override
    {
        SDL_Init(SDL_INIT_VIDEO);
        // SDL_GetKeyboardState returns a pointer to SDL's internal array.
        // We cast away const so we can inject key states for testing.
        mKeyState = const_cast<Uint8 *>(SDL_GetKeyboardState(&mNumKeys));
        clearAllKeys();
    }

    void TearDown() override
    {
        clearAllKeys();
        SDL_Quit();
    }

    void clearAllKeys()
    {
        if (mKeyState)
        {
            std::memset(mKeyState, 0, mNumKeys);
        }
    }

    void setKey(SDL_Scancode key, bool pressed)
    {
        mKeyState[key] = pressed ? 1 : 0;
    }

    /// Inject a key state change into both SDL's array and the InputManager's event buffer
    void simulateKey(InputManager &input, SDL_Scancode key, bool pressed)
    {
        setKey(key, pressed);

        SDL_Event event;
        std::memset(&event, 0, sizeof(event));
        event.type = pressed ? SDL_KEYDOWN : SDL_KEYUP;
        event.key.keysym.scancode = key;
        event.key.repeat = 0;
        input.processEvent(event);
    }

    /// Simulate a frame boundary: clear buffers
    void nextFrame(InputManager &input)
    {
        input.clearBuffers();
    }
};

// ---- Tests for initial / idle state ----

TEST_F(InputManagerTest, InitialStateBeforeUpdate)
{
    InputManager input;

    // Before any update(), all queries should return false
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, UpdatePopulatesKeyState)
{
    InputManager input;
    nextFrame(input);

    // After update, keyboard state pointer is valid — no keys should be pressed
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_ESCAPE));
}

TEST_F(InputManagerTest, NoSpuriousPressAfterFirstUpdate)
{
    InputManager input;
    nextFrame(input);

    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_RETURN));
}

TEST_F(InputManagerTest, NoSpuriousReleaseAfterFirstUpdate)
{
    InputManager input;
    nextFrame(input);

    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_RETURN));
}

TEST_F(InputManagerTest, ConsecutiveUpdatesWithNoInput)
{
    InputManager input;
    nextFrame(input);
    nextFrame(input);
    nextFrame(input);

    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_W));
}

TEST_F(InputManagerTest, MultipleKeysAllIdle)
{
    InputManager input;
    nextFrame(input);

    SDL_Scancode keys[] = {
        SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_W,
        SDL_SCANCODE_SPACE, SDL_SCANCODE_ESCAPE, SDL_SCANCODE_LSHIFT};

    for (auto key : keys)
    {
        EXPECT_FALSE(input.isKeyDown(key));
        EXPECT_FALSE(input.isKeyPressed(key));
        EXPECT_FALSE(input.isKeyReleased(key));
    }
}

// ---- Tests for key press detection ----

TEST_F(InputManagerTest, KeyDownDetected)
{
    InputManager input;
    nextFrame(input); // previous = all 0, current ptr set

    // Inject key A into SDL's live array and event buffer
    simulateKey(input, SDL_SCANCODE_A, true);

    // Now current[A]=1, buffered press[A]=1
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, KeyHeldDownAcrossFrames)
{
    InputManager input;
    nextFrame(input);

    simulateKey(input, SDL_SCANCODE_A, true);
    // Frame 1: buffered press[A]=1
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));

    nextFrame(input);
    // Frame 2: key still held but no new event → not "just pressed"
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, KeyReleaseDetected)
{
    InputManager input;
    nextFrame(input);

    // Press A
    simulateKey(input, SDL_SCANCODE_A, true);
    nextFrame(input); // snapshot: previous[A]=1

    // Release A
    simulateKey(input, SDL_SCANCODE_A, false);

    // Now current[A]=0, buffered release[A]=1
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, ReleaseStateLastsOneFrame)
{
    InputManager input;
    nextFrame(input);

    simulateKey(input, SDL_SCANCODE_A, true);
    nextFrame(input); // previous[A]=1

    simulateKey(input, SDL_SCANCODE_A, false);
    // Released this "frame"
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_A));

    nextFrame(input); // buffers cleared

    // Next frame — no longer released
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, PressStateLastsOneFrame)
{
    InputManager input;
    nextFrame(input);

    simulateKey(input, SDL_SCANCODE_A, true);
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));

    nextFrame(input); // buffers cleared, key still held

    // Still held but not "just pressed"
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, MultipleKeysIndependent)
{
    InputManager input;
    nextFrame(input);

    simulateKey(input, SDL_SCANCODE_A, true);
    simulateKey(input, SDL_SCANCODE_W, true);

    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_D));

    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_D));
}

TEST_F(InputManagerTest, ReleaseOneKeyWhileOtherHeld)
{
    InputManager input;
    nextFrame(input);

    // Press A and W
    simulateKey(input, SDL_SCANCODE_A, true);
    simulateKey(input, SDL_SCANCODE_W, true);
    nextFrame(input); // previous: A=1, W=1

    // Release only A
    simulateKey(input, SDL_SCANCODE_A, false);

    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_A));

    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_W)); // Not newly pressed
}

TEST_F(InputManagerTest, FullPressHoldReleaseCycle)
{
    InputManager input;
    nextFrame(input); // Frame 0: baseline

    // Frame 1: press
    simulateKey(input, SDL_SCANCODE_SPACE, true);
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_SPACE));

    // Frame 2: hold
    nextFrame(input);
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_SPACE));

    // Frame 3: release
    simulateKey(input, SDL_SCANCODE_SPACE, false);
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_SPACE));

    // Frame 4: idle
    nextFrame(input);
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_SPACE));
}

TEST_F(InputManagerTest, UnrelatedKeysNotAffected)
{
    InputManager input;
    nextFrame(input);

    setKey(SDL_SCANCODE_A, true);

    // Only A should show state changes
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_B));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_B));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_B));
}
