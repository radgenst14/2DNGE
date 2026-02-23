#include <gtest/gtest.h>
#include "engine/scripting/ScriptEngine.h"
#include "engine/scripting/EngineBindings.h"
#include "engine/core/ecs/EntityManager.h"
#include "engine/core/ecs/components/Transform.h"
#include "engine/core/ecs/components/RigidBody.h"
#include "engine/core/InputManager.h"

class EngineBindingsTest : public ::testing::Test
{
protected:
    EntityManager em;
    ScriptEngine se;

    void SetUp() override
    {
        EngineBindings::setEntityManager(&em);
    }

    void TearDown() override
    {
        EngineBindings::setEntityManager(nullptr);
        EngineBindings::setPhysicsManager(nullptr);
        EngineBindings::setRenderManager(nullptr);
        EngineBindings::setInputManager(nullptr);
    }
};

TEST_F(EngineBindingsTest, CreateEntityFromPython)
{
    se.execute("import engine");
    se.execute("engine.create_entity()");

    // Entity 0 should exist — create another to verify the counter advanced
    EntityID e = em.createEntity();
    EXPECT_EQ(e, 1);
}

TEST_F(EngineBindingsTest, AddTransformFromPython)
{
    se.execute("import engine");
    se.execute("e = engine.create_entity()");
    se.execute("engine.add_transform(e, 100.0, 200.0)");

    auto &t = em.getComponent<ECS::Transform>(0);
    EXPECT_FLOAT_EQ(t.position.x, 100.0f);
    EXPECT_FLOAT_EQ(t.position.y, 200.0f);
}

TEST_F(EngineBindingsTest, AddRigidBodyFromPython)
{
    se.execute("import engine");
    se.execute("e = engine.create_entity()");
    se.execute("engine.add_transform(e, 0.0, 0.0)");
    se.execute("engine.add_rigidbody(e, 5.0, -3.0, 2.0)");

    auto &rb = em.getComponent<ECS::RigidBody>(0);
    EXPECT_FLOAT_EQ(rb.velocity.x, 5.0f);
    EXPECT_FLOAT_EQ(rb.velocity.y, -3.0f);
    EXPECT_FLOAT_EQ(rb.mass, 2.0f);
}

TEST_F(EngineBindingsTest, GetPositionFromPython)
{
    se.execute("import engine");
    se.execute("e = engine.create_entity()");
    se.execute("engine.add_transform(e, 42.0, 99.0)");
    auto result = se.execute("engine.get_position(e)");

    auto tuple = result.cast<py::tuple>();
    EXPECT_FLOAT_EQ(tuple[0].cast<float>(), 42.0f);
    EXPECT_FLOAT_EQ(tuple[1].cast<float>(), 99.0f);
}

TEST_F(EngineBindingsTest, FullScriptCreatesEntity)
{
    EngineBindings::setEntityManager(&em);

    se.loadScript("../../testing/test_scripting/test_bindings.py");
    se.callFunction("setup");

    EXPECT_TRUE(em.hasComponent<ECS::Transform>(0));
    EXPECT_TRUE(em.hasComponent<ECS::RigidBody>(0));

    auto &t = em.getComponent<ECS::Transform>(0);
    EXPECT_FLOAT_EQ(t.position.x, 50.0f);
    EXPECT_FLOAT_EQ(t.position.y, 75.0f);
}

// ===========================================================================
// InputManager bindings
// ===========================================================================

TEST_F(EngineBindingsTest, SetAndGetInputManager)
{
    InputManager im;
    EngineBindings::setInputManager(&im);

    EXPECT_EQ(EngineBindings::getInputManager(), &im);

    EngineBindings::setInputManager(nullptr);
    EXPECT_EQ(EngineBindings::getInputManager(), nullptr);
}

TEST_F(EngineBindingsTest, InputManagerInitiallyNull)
{
    // TearDown resets it, so at the start of each test it should be null
    // (unless SetUp sets it). InputManager is not set in SetUp.
    EXPECT_EQ(EngineBindings::getInputManager(), nullptr);
}

// ===========================================================================
// RenderManager bindings
// ===========================================================================

TEST_F(EngineBindingsTest, SetAndGetRenderManager)
{
    // We can't easily create a real RenderManager without a Window/SDL.
    // But we can test the set/get with nullptr round-trip.
    EngineBindings::setRenderManager(nullptr);
    EXPECT_EQ(EngineBindings::getRenderManager(), nullptr);
}

TEST_F(EngineBindingsTest, RenderManagerInitiallyNull)
{
    EXPECT_EQ(EngineBindings::getRenderManager(), nullptr);
}

// ===========================================================================
// PhysicsManager bindings
// ===========================================================================

TEST_F(EngineBindingsTest, SetAndGetPhysicsManager)
{
    EngineBindings::setPhysicsManager(nullptr);
    EXPECT_EQ(EngineBindings::getPhysicsManager(), nullptr);
}

// ===========================================================================
// Multiple bindings coexist
// ===========================================================================

TEST_F(EngineBindingsTest, AllBindingsCoexist)
{
    InputManager im;

    EngineBindings::setEntityManager(&em);
    EngineBindings::setInputManager(&im);

    // All should be independently accessible
    EXPECT_EQ(EngineBindings::getEntityManager(), &em);
    EXPECT_EQ(EngineBindings::getInputManager(), &im);

    // Clearing one doesn't affect the other
    EngineBindings::setInputManager(nullptr);
    EXPECT_EQ(EngineBindings::getEntityManager(), &em);
    EXPECT_EQ(EngineBindings::getInputManager(), nullptr);
}

// ===========================================================================
// Python render binding is registered
// ===========================================================================

TEST_F(EngineBindingsTest, RenderFunctionExistsInPython)
{
    // The render() function should be available in the engine module
    // even if RenderManager is null (it should handle null gracefully)
    se.execute("import engine");
    // Calling engine.render() with null RenderManager should not crash
    se.execute("engine.render()");
}

TEST_F(EngineBindingsTest, PhysicsUpdateExistsInPython)
{
    // physics_update requires a valid PhysicsManager; just verify it's importable
    se.execute("import engine");
    EXPECT_TRUE(se.execute("hasattr(engine, 'physics_update')").cast<bool>());
}

TEST_F(EngineBindingsTest, RenderAttrExistsInPython)
{
    se.execute("import engine");
    EXPECT_TRUE(se.execute("hasattr(engine, 'render')").cast<bool>());
}

// ===========================================================================
// Sprite bindings
// ===========================================================================

TEST_F(EngineBindingsTest, AddSpriteAttrExistsInPython)
{
    se.execute("import engine");
    EXPECT_TRUE(se.execute("hasattr(engine, 'add_sprite')").cast<bool>());
}

TEST_F(EngineBindingsTest, AddSpriteThrowsOnMissingTexture)
{
    se.execute("import engine");
    se.execute("e = engine.create_entity()");

    EXPECT_THROW(se.execute("engine.add_sprite(e, 'nonexistent')"), py::error_already_set);
}