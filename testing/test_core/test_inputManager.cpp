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
    input.update();

    // After update, keyboard state pointer is valid — no keys should be pressed
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_ESCAPE));
}

TEST_F(InputManagerTest, NoSpuriousPressAfterFirstUpdate)
{
    InputManager input;
    input.update();

    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_RETURN));
}

TEST_F(InputManagerTest, NoSpuriousReleaseAfterFirstUpdate)
{
    InputManager input;
    input.update();

    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_RETURN));
}

TEST_F(InputManagerTest, ConsecutiveUpdatesWithNoInput)
{
    InputManager input;
    input.update();
    input.update();
    input.update();

    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_W));
}

TEST_F(InputManagerTest, MultipleKeysAllIdle)
{
    InputManager input;
    input.update();

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
//
// Because InputManager::update() snapshots the SDL internal array into
// mPreviousKeyState and then reads the same array as mCurrentKeyState,
// we set keys AFTER calling update() so that:
//   previous = snapshot before the change, current = live array with change.

TEST_F(InputManagerTest, KeyDownDetected)
{
    InputManager input;
    input.update(); // previous = all 0, current ptr set

    // Inject key A into SDL's live array
    setKey(SDL_SCANCODE_A, true);

    // Now current[A]=1, previous[A]=0
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, KeyHeldDownAcrossFrames)
{
    InputManager input;
    input.update();

    setKey(SDL_SCANCODE_A, true);
    // Frame 1: current[A]=1, previous[A]=0 → pressed
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));

    input.update();
    // Frame 2: previous[A]=1 (snapshotted), current[A]=1 (still held)
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, KeyReleaseDetected)
{
    InputManager input;
    input.update();

    // Press A
    setKey(SDL_SCANCODE_A, true);
    input.update(); // snapshot: previous[A]=1

    // Release A
    setKey(SDL_SCANCODE_A, false);

    // Now current[A]=0, previous[A]=1 → released
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, ReleaseStateLastsOneFrame)
{
    InputManager input;
    input.update();

    setKey(SDL_SCANCODE_A, true);
    input.update(); // previous[A]=1

    setKey(SDL_SCANCODE_A, false);
    // Released this "frame"
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_A));

    input.update(); // previous[A]=0 now

    // Next frame — no longer released
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, PressStateLastsOneFrame)
{
    InputManager input;
    input.update();

    setKey(SDL_SCANCODE_A, true);
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_A));

    input.update(); // previous now has A=1

    // Still held but not "just pressed"
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_A));
}

TEST_F(InputManagerTest, MultipleKeysIndependent)
{
    InputManager input;
    input.update();

    setKey(SDL_SCANCODE_A, true);
    setKey(SDL_SCANCODE_W, true);

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
    input.update();

    // Press A and W
    setKey(SDL_SCANCODE_A, true);
    setKey(SDL_SCANCODE_W, true);
    input.update(); // previous: A=1, W=1

    // Release only A
    setKey(SDL_SCANCODE_A, false);

    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_A));

    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_W));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_W)); // Not newly pressed
}

TEST_F(InputManagerTest, FullPressHoldReleaseCycle)
{
    InputManager input;
    input.update(); // Frame 0: baseline

    // Frame 1: press
    setKey(SDL_SCANCODE_SPACE, true);
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_SPACE));

    // Frame 2: hold
    input.update();
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_SPACE));

    // Frame 3: release
    setKey(SDL_SCANCODE_SPACE, false);
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(input.isKeyReleased(SDL_SCANCODE_SPACE));

    // Frame 4: idle
    input.update();
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_SPACE));
}

TEST_F(InputManagerTest, UnrelatedKeysNotAffected)
{
    InputManager input;
    input.update();

    setKey(SDL_SCANCODE_A, true);

    // Only A should show state changes
    EXPECT_TRUE(input.isKeyDown(SDL_SCANCODE_A));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_B));
    EXPECT_FALSE(input.isKeyDown(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(input.isKeyPressed(SDL_SCANCODE_B));
    EXPECT_FALSE(input.isKeyReleased(SDL_SCANCODE_B));
}
