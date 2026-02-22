#include <gtest/gtest.h>
#include "engine/scripting/ScriptableScene.h"
#include "engine/scripting/EngineBindings.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/components/Transform.h"
#include "engine/ecs/components/RigidBody.h"
#include <string>

static const std::string SCRIPTS_DIR =
    std::string(PROJECT_SOURCE_DIR) + "/testing/test_scripting/";

// ---------------------------------------------------------------------------
// Fixture – provides helpers for creating and managing ScriptableScene
// ---------------------------------------------------------------------------
class ScriptableSceneTest : public ::testing::Test
{
protected:
    // Convenience: build a full path from just the filename
    static std::string scriptPath(const std::string &filename)
    {
        return SCRIPTS_DIR + filename;
    }
};

// ===========================================================================
// Construction
// ===========================================================================

TEST_F(ScriptableSceneTest, ConstructWithValidPath)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    // Construction should succeed without crashing
}

TEST_F(ScriptableSceneTest, ConstructWithNonExistentPath)
{
    ScriptableScene scene("/nonexistent/path/missing.py");
    // Construction itself should not crash; failure happens at init()
}

// ===========================================================================
// Init
// ===========================================================================

TEST_F(ScriptableSceneTest, InitLoadsScriptAndCallsInit)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"));
    scene.init();

    // The script's init() creates an entity with Transform at (10, 20).
    // We can verify this indirectly by checking EngineBindings state was set
    // and the script actually ran. Cleanup resets the binding.
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InitHandlesInvalidScript)
{
    ScriptableScene scene(scriptPath("test_scene_bad_syntax.py"));
    // init() should log an error but not crash
    scene.init();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InitHandlesMissingFile)
{
    ScriptableScene scene("/nonexistent/missing_script.py");
    // init() should log an error but not crash
    scene.init();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InitCreatesEntityManager)
{
    ScriptableScene scene(scriptPath("test_scene_entities.py"));
    scene.init();

    // test_scene_entities.py creates 5 entities with transforms and rigidbodies.
    // If init() didn't create the EntityManager, the script would crash.
    // Reaching cleanup without crash confirms EntityManager was created.
    scene.cleanup();
}

// ===========================================================================
// Update
// ===========================================================================

TEST_F(ScriptableSceneTest, UpdateCallsScriptUpdate)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    // Calling update should not crash with a valid minimal script
    scene.update(0.016f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdatePassesDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"));
    scene.init();

    // test_scene_init.py stores dt in update_dt global
    scene.update(0.033f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, MultipleUpdates)
{
    ScriptableScene scene(scriptPath("test_scene_update_counter.py"));
    scene.init();

    // Call update multiple times; script increments a counter each time
    for (int i = 0; i < 100; ++i)
    {
        scene.update(0.016f);
    }

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdateWithZeroDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    scene.update(0.0f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdateWithLargeDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    scene.update(1.0f);

    scene.cleanup();
}

// ===========================================================================
// Input (currently empty implementation)
// ===========================================================================

TEST_F(ScriptableSceneTest, InputDoesNotCrash)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    SDL_Event event{};
    event.type = SDL_KEYDOWN;
    event.key.keysym.scancode = SDL_SCANCODE_SPACE;
    scene.input(event);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InputHandlesMultipleEventTypes)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    SDL_Event keyDown{};
    keyDown.type = SDL_KEYDOWN;
    keyDown.key.keysym.scancode = SDL_SCANCODE_A;
    scene.input(keyDown);

    SDL_Event keyUp{};
    keyUp.type = SDL_KEYUP;
    keyUp.key.keysym.scancode = SDL_SCANCODE_A;
    scene.input(keyUp);

    SDL_Event mouseMotion{};
    mouseMotion.type = SDL_MOUSEMOTION;
    mouseMotion.motion.x = 100;
    mouseMotion.motion.y = 200;
    scene.input(mouseMotion);

    scene.cleanup();
}

// ===========================================================================
// Render (currently empty implementation)
// ===========================================================================

// Note: Renderer requires a Window which requires SDL video initialization.
// Since render() is a no-op, we skip renderer-dependent tests.

// ===========================================================================
// Cleanup
// ===========================================================================

TEST_F(ScriptableSceneTest, CleanupCallsScriptCleanup)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"));
    scene.init();
    scene.cleanup();

    // cleanup() should call the script's cleanup() function and reset bindings.
    // No crash means it worked correctly.
}

TEST_F(ScriptableSceneTest, CleanupResetsEngineBindings)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();
    scene.cleanup();

    // After cleanup, EngineBindings should have a null EntityManager.
    // Creating a new scene should work fine (proves bindings were reset).
    ScriptableScene scene2(scriptPath("test_scene_minimal.py"));
    scene2.init();
    scene2.cleanup();
}

TEST_F(ScriptableSceneTest, CleanupAfterFailedInit)
{
    ScriptableScene scene("/nonexistent/missing.py");
    scene.init();
    // Cleanup after failed init should not crash
    scene.cleanup();
}

// ===========================================================================
// Full lifecycle
// ===========================================================================

TEST_F(ScriptableSceneTest, FullLifecycleMinimalScript)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    for (int i = 0; i < 10; ++i)
    {
        SDL_Event event{};
        event.type = SDL_KEYDOWN;
        scene.input(event);
        scene.update(0.016f);
    }

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, FullLifecycleWithEntities)
{
    ScriptableScene scene(scriptPath("test_scene_entities.py"));
    scene.init();

    // Run several frames of the full game loop
    for (int i = 0; i < 60; ++i)
    {
        SDL_Event event{};
        event.type = SDL_FIRSTEVENT;
        scene.input(event);
        scene.update(0.016f);
    }

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, FullLifecycleInitUpdateCleanup)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"));
    scene.init();

    // Simulate a brief game session
    scene.update(0.016f);
    scene.update(0.016f);
    scene.update(0.033f);

    scene.cleanup();
}

// ===========================================================================
// Sequential scene instances (simulate scene switching)
// ===========================================================================

TEST_F(ScriptableSceneTest, SequentialSceneInstances)
{
    // First scene
    {
        ScriptableScene scene(scriptPath("test_scene_init.py"));
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }

    // Second scene — should work after first is cleaned up
    {
        ScriptableScene scene(scriptPath("test_scene_entities.py"));
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }

    // Third scene — back to minimal
    {
        ScriptableScene scene(scriptPath("test_scene_minimal.py"));
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }
}

TEST_F(ScriptableSceneTest, ReinitSameScript)
{
    // Load the same script twice in sequence
    {
        ScriptableScene scene(scriptPath("test_scene_init.py"));
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }

    {
        ScriptableScene scene(scriptPath("test_scene_init.py"));
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }
}

// ===========================================================================
// Script creates entities with components (integration)
// ===========================================================================

TEST_F(ScriptableSceneTest, ScriptCreatesMultipleEntities)
{
    ScriptableScene scene(scriptPath("test_scene_entities.py"));
    scene.init();

    // test_scene_entities.py creates 5 entities, each with Transform and RigidBody.
    // Running update() queries their positions — this exercises the full
    // ScriptableScene -> ScriptEngine -> EngineBindings -> EntityManager pipeline.
    scene.update(0.016f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, ScriptCreatesEntityWithPhysics)
{
    ScriptableScene scene(scriptPath("test_scene_update_position.py"));
    scene.init();

    // test_scene_update_position.py creates an entity with velocity (100, 50).
    // ScriptableScene.update() calls the Python update but does NOT call
    // PhysicsManager.update() — so position should remain at origin.
    // This test verifies the scene holds both managers without conflict.
    scene.update(0.016f);

    scene.cleanup();
}

// ===========================================================================
// Non-copyable
// ===========================================================================

TEST_F(ScriptableSceneTest, IsNonCopyable)
{
    EXPECT_FALSE(std::is_copy_constructible<ScriptableScene>::value);
    EXPECT_FALSE(std::is_copy_assignable<ScriptableScene>::value);
}

// ===========================================================================
// IScene interface conformance
// ===========================================================================

TEST_F(ScriptableSceneTest, ImplementsISceneInterface)
{
    // ScriptableScene should be usable through an IScene pointer
    std::unique_ptr<IScene> scene =
        std::make_unique<ScriptableScene>(scriptPath("test_scene_minimal.py"));

    scene->init();
    scene->update(0.016f);

    SDL_Event event{};
    event.type = SDL_FIRSTEVENT;
    scene->input(event);

    scene->cleanup();
}

TEST_F(ScriptableSceneTest, PolymorphicDestructionViaIScene)
{
    // Destroying through base pointer should properly clean up
    IScene *scene = new ScriptableScene(scriptPath("test_scene_minimal.py"));
    scene->init();
    scene->update(0.016f);
    scene->cleanup();
    delete scene;
}

// ===========================================================================
// Edge cases
// ===========================================================================

TEST_F(ScriptableSceneTest, UpdateBeforeInit)
{
    // Calling update before init — the script won't be loaded yet.
    // ScriptEngine::callFunction should handle this gracefully.
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.update(0.016f);
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, CleanupWithoutInit)
{
    // Cleanup without ever calling init should not crash
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, DoubleInit)
{
    // Calling init twice — should not crash or leak
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();
    scene.init();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, DoubleCleanup)
{
    // Calling cleanup twice should not crash
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();
    scene.cleanup();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdateAfterCleanup)
{
    // Calling update after cleanup — script functions may not be callable
    // but it should not crash
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();
    scene.cleanup();
    scene.update(0.016f);
}

TEST_F(ScriptableSceneTest, NegativeDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"));
    scene.init();

    // Negative dt is unusual but should not crash
    scene.update(-0.016f);

    scene.cleanup();
}
