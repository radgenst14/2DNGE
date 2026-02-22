#include <gtest/gtest.h>
#include "engine/scripting/ScriptEngine.h"
#include "engine/scripting/EngineBindings.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/components/Transform.h"
#include "engine/ecs/components/RigidBody.h"

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