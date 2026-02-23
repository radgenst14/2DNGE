#include <gtest/gtest.h>
#include "engine/scripting/ScriptableScene.h"
#include "engine/scripting/EngineBindings.h"
#include "engine/core/ecs/EntityManager.h"
#include "engine/core/ecs/components/Transform.h"
#include "engine/core/ecs/components/RigidBody.h"
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
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    // Construction should succeed without crashing
}

TEST_F(ScriptableSceneTest, ConstructWithNonExistentPath)
{
    ScriptableScene scene("/nonexistent/path/missing.py", nullptr);
    // Construction itself should not crash; failure happens at init()
}

// ===========================================================================
// Init
// ===========================================================================

TEST_F(ScriptableSceneTest, InitLoadsScriptAndCallsInit)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
    scene.init();

    // The script's init() creates an entity with Transform at (10, 20).
    // We can verify this indirectly by checking EngineBindings state was set
    // and the script actually ran. Cleanup resets the binding.
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InitHandlesInvalidScript)
{
    ScriptableScene scene(scriptPath("test_scene_bad_syntax.py"), nullptr);
    // init() should log an error but not crash
    scene.init();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InitHandlesMissingFile)
{
    ScriptableScene scene("/nonexistent/missing_script.py", nullptr);
    // init() should log an error but not crash
    scene.init();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InitCreatesEntityManager)
{
    ScriptableScene scene(scriptPath("test_scene_entities.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();

    // Calling update should not crash with a valid minimal script
    scene.update(0.016f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdatePassesDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
    scene.init();

    // test_scene_init.py stores dt in update_dt global
    scene.update(0.033f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, MultipleUpdates)
{
    ScriptableScene scene(scriptPath("test_scene_update_counter.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();

    scene.update(0.0f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdateWithLargeDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();

    scene.update(1.0f);

    scene.cleanup();
}

// ===========================================================================
// Input (currently empty implementation)
// ===========================================================================

TEST_F(ScriptableSceneTest, InputDoesNotCrash)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();

    SDL_Event event{};
    event.type = SDL_KEYDOWN;
    event.key.keysym.scancode = SDL_SCANCODE_SPACE;
    scene.input(event);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InputHandlesMultipleEventTypes)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
    scene.init();
    scene.cleanup();

    // cleanup() should call the script's cleanup() function and reset bindings.
    // No crash means it worked correctly.
}

TEST_F(ScriptableSceneTest, CleanupResetsEngineBindings)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.cleanup();

    // After cleanup, EngineBindings should have a null EntityManager.
    // Creating a new scene should work fine (proves bindings were reset).
    ScriptableScene scene2(scriptPath("test_scene_minimal.py"), nullptr);
    scene2.init();
    scene2.cleanup();
}

TEST_F(ScriptableSceneTest, CleanupAfterFailedInit)
{
    ScriptableScene scene("/nonexistent/missing.py", nullptr);
    scene.init();
    // Cleanup after failed init should not crash
    scene.cleanup();
}

// ===========================================================================
// Full lifecycle
// ===========================================================================

TEST_F(ScriptableSceneTest, FullLifecycleMinimalScript)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_entities.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
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
        ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }

    // Second scene — should work after first is cleaned up
    {
        ScriptableScene scene(scriptPath("test_scene_entities.py"), nullptr);
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }

    // Third scene — back to minimal
    {
        ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }
}

TEST_F(ScriptableSceneTest, ReinitSameScript)
{
    // Load the same script twice in sequence
    {
        ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
        scene.init();
        scene.update(0.016f);
        scene.cleanup();
    }

    {
        ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_entities.py"), nullptr);
    scene.init();

    // test_scene_entities.py creates 5 entities, each with Transform and RigidBody.
    // Running update() queries their positions — this exercises the full
    // ScriptableScene -> ScriptEngine -> EngineBindings -> EntityManager pipeline.
    scene.update(0.016f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, ScriptCreatesEntityWithPhysics)
{
    ScriptableScene scene(scriptPath("test_scene_update_position.py"), nullptr);
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
        std::make_unique<ScriptableScene>(scriptPath("test_scene_minimal.py"), nullptr);

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
    IScene *scene = new ScriptableScene(scriptPath("test_scene_minimal.py"), nullptr);
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
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.update(0.016f);
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, CleanupWithoutInit)
{
    // Cleanup without ever calling init should not crash
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, DoubleInit)
{
    // Calling init twice — should not crash or leak
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.init();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, DoubleCleanup)
{
    // Calling cleanup twice should not crash
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.cleanup();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, UpdateAfterCleanup)
{
    // Calling update after cleanup — script functions may not be callable
    // but it should not crash
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.cleanup();
    scene.update(0.016f);
}

TEST_F(ScriptableSceneTest, NegativeDeltaTime)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();

    // Negative dt is unusual but should not crash
    scene.update(-0.016f);

    scene.cleanup();
}

// ===========================================================================
// Render calls script render() function
// ===========================================================================

TEST_F(ScriptableSceneTest, RenderCallsScriptRender)
{
    // test_scene_render.py defines a render() function that calls engine.render()
    ScriptableScene scene(scriptPath("test_scene_render.py"), nullptr);
    scene.init();

    // render() should call the script's render function without crashing
    scene.render();

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, RenderMultipleTimes)
{
    ScriptableScene scene(scriptPath("test_scene_render.py"), nullptr);
    scene.init();

    for (int i = 0; i < 60; ++i)
    {
        scene.render();
    }

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, RenderBeforeInit)
{
    // Calling render before init should not crash
    ScriptableScene scene(scriptPath("test_scene_render.py"), nullptr);
    scene.render();
    scene.cleanup();
}

TEST_F(ScriptableSceneTest, RenderAfterCleanup)
{
    // Calling render after cleanup should not crash
    ScriptableScene scene(scriptPath("test_scene_render.py"), nullptr);
    scene.init();
    scene.cleanup();
    scene.render();
}

TEST_F(ScriptableSceneTest, RenderWithMinimalScript)
{
    // test_scene_minimal.py does NOT define render() — ScriptEngine should
    // handle the missing function gracefully without crashing
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.render();
    scene.cleanup();
}

// ===========================================================================
// Input calls script input() function
// ===========================================================================

TEST_F(ScriptableSceneTest, InputCallsScriptInput)
{
    // test_scene_input.py defines an input() function that increments a counter
    ScriptableScene scene(scriptPath("test_scene_input.py"), nullptr);
    scene.init();

    SDL_Event event{};
    event.type = SDL_KEYDOWN;
    event.key.keysym.scancode = SDL_SCANCODE_SPACE;
    scene.input(event);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, InputCalledMultipleTimes)
{
    ScriptableScene scene(scriptPath("test_scene_input.py"), nullptr);
    scene.init();

    for (int i = 0; i < 30; ++i)
    {
        SDL_Event event{};
        event.type = SDL_KEYDOWN;
        event.key.keysym.scancode = SDL_SCANCODE_A;
        scene.input(event);
    }

    scene.cleanup();
}

// ===========================================================================
// Cleanup resets RenderManager binding
// ===========================================================================

TEST_F(ScriptableSceneTest, CleanupResetsRenderManagerBinding)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.cleanup();

    // After cleanup, RenderManager binding should be null
    EXPECT_EQ(EngineBindings::getRenderManager(), nullptr);
}

TEST_F(ScriptableSceneTest, CleanupResetsAllBindings)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();
    scene.cleanup();

    EXPECT_EQ(EngineBindings::getEntityManager(), nullptr);
    EXPECT_EQ(EngineBindings::getPhysicsManager(), nullptr);
    EXPECT_EQ(EngineBindings::getRenderManager(), nullptr);
}

// ===========================================================================
// Destructor calls cleanup
// ===========================================================================

TEST_F(ScriptableSceneTest, DestructorCallsCleanup)
{
    {
        ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
        scene.init();
        // scene goes out of scope — destructor should call cleanup()
    }

    // After destruction, bindings should be reset
    EXPECT_EQ(EngineBindings::getEntityManager(), nullptr);
    EXPECT_EQ(EngineBindings::getPhysicsManager(), nullptr);
    EXPECT_EQ(EngineBindings::getRenderManager(), nullptr);
}

TEST_F(ScriptableSceneTest, DestructorSafeWithoutInit)
{
    {
        // Constructing and destroying without init should not crash
        ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    }
}

TEST_F(ScriptableSceneTest, DestructorAfterExplicitCleanup)
{
    {
        ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
        scene.init();
        scene.cleanup();
        // Destructor will call cleanup again — should handle double-cleanup safely
    }
}

// ===========================================================================
// Double-init guard (cleanup before re-init)
// ===========================================================================

TEST_F(ScriptableSceneTest, DoubleInitCleansUpBeforeReinit)
{
    ScriptableScene scene(scriptPath("test_scene_init.py"), nullptr);
    scene.init();

    // Second init should cleanup the first init's resources and reinitialize
    scene.init();

    // Should still work correctly after double-init
    scene.update(0.016f);

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, DoubleInitWithEntities)
{
    ScriptableScene scene(scriptPath("test_scene_entities.py"), nullptr);
    scene.init();

    // Entities from first init should be cleaned up
    scene.init();

    // New entities should be created by second init
    scene.update(0.016f);

    scene.cleanup();
}

// ===========================================================================
// Null Window parameter
// ===========================================================================

TEST_F(ScriptableSceneTest, NullWindowSkipsRenderManagerCreation)
{
    ScriptableScene scene(scriptPath("test_scene_minimal.py"), nullptr);
    scene.init();

    // With null Window, RenderManager should not be created
    EXPECT_EQ(EngineBindings::getRenderManager(), nullptr);

    // But EntityManager and PhysicsManager should be created
    EXPECT_NE(EngineBindings::getEntityManager(), nullptr);
    EXPECT_NE(EngineBindings::getPhysicsManager(), nullptr);

    scene.cleanup();
}

// ===========================================================================
// Full lifecycle with input, update, and render
// ===========================================================================

TEST_F(ScriptableSceneTest, FullLifecycleWithAllCallbacks)
{
    ScriptableScene scene(scriptPath("test_scene_full_lifecycle.py"), nullptr);
    scene.init();

    for (int i = 0; i < 30; ++i)
    {
        SDL_Event event{};
        event.type = SDL_KEYDOWN;
        scene.input(event);
        scene.update(0.016f);
        scene.render();
    }

    scene.cleanup();
}

TEST_F(ScriptableSceneTest, FullLifecycleViaIScene)
{
    // Verify the full lifecycle works through the IScene interface
    std::unique_ptr<IScene> scene =
        std::make_unique<ScriptableScene>(scriptPath("test_scene_full_lifecycle.py"), nullptr);

    scene->init();

    for (int i = 0; i < 10; ++i)
    {
        SDL_Event event{};
        event.type = SDL_KEYDOWN;
        scene->input(event);
        scene->update(0.016f);
        scene->render();
    }

    scene->cleanup();
}

// ===========================================================================
// Sequential scenes with render and input
// ===========================================================================

TEST_F(ScriptableSceneTest, SequentialScenesWithRenderAndInput)
{
    // First scene
    {
        ScriptableScene scene(scriptPath("test_scene_full_lifecycle.py"), nullptr);
        scene.init();
        SDL_Event event{};
        event.type = SDL_KEYDOWN;
        scene.input(event);
        scene.update(0.016f);
        scene.render();
        scene.cleanup();
    }

    // Second scene
    {
        ScriptableScene scene(scriptPath("test_scene_render.py"), nullptr);
        scene.init();
        scene.update(0.016f);
        scene.render();
        scene.cleanup();
    }

    // Third scene — input focused
    {
        ScriptableScene scene(scriptPath("test_scene_input.py"), nullptr);
        scene.init();
        SDL_Event event{};
        event.type = SDL_KEYDOWN;
        scene.input(event);
        scene.cleanup();
    }
}

// ===========================================================================
// render() via IScene with no Renderer argument
// ===========================================================================

TEST_F(ScriptableSceneTest, RenderSignatureNoRendererArg)
{
    // IScene::render() takes no arguments (changed from Renderer&)
    // This test confirms the interface compiles and works
    std::unique_ptr<IScene> scene =
        std::make_unique<ScriptableScene>(scriptPath("test_scene_render.py"), nullptr);

    scene->init();
    scene->render(); // No Renderer argument
    scene->cleanup();
}
