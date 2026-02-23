#include <gtest/gtest.h>

#include "engine/core/ecs/components/Transform.h"
#include "engine/core/ecs/components/RigidBody.h"
#include "engine/physics/PhysicsManager.h"
#include "engine/core/ecs/EntityManager.h"

TEST(PhysicsManagerTest, UpdateMovesEntityRight)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e, ECS::RigidBody{{1.0f, 0.0f}});

    pm.update(1.0f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 1.0f);
    EXPECT_FLOAT_EQ(t.position.y, 0.0f);
}

TEST(PhysicsManagerTest, UpdateMovesEntityUp)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e, ECS::RigidBody{{0.0f, -5.0f}});

    pm.update(2.0f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 0.0f);
    EXPECT_FLOAT_EQ(t.position.y, -10.0f);
}

TEST(PhysicsManagerTest, UpdateMovesEntityDiagonally)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e, ECS::RigidBody{{3.0f, 4.0f}});

    pm.update(0.5f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 1.5f);
    EXPECT_FLOAT_EQ(t.position.y, 2.0f);
}

TEST(PhysicsManagerTest, ZeroVelocityNoMovement)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{5.0f, 5.0f}});
    em.addComponent(e, ECS::RigidBody{{0.0f, 0.0f}});

    pm.update(10.0f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 5.0f);
    EXPECT_FLOAT_EQ(t.position.y, 5.0f);
}

TEST(PhysicsManagerTest, ZeroDeltaTimeNoMovement)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e, ECS::RigidBody{{100.0f, 100.0f}});

    pm.update(0.0f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 0.0f);
    EXPECT_FLOAT_EQ(t.position.y, 0.0f);
}

TEST(PhysicsManagerTest, MultipleEntitiesUpdate)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e1 = em.createEntity();
    em.addComponent(e1, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e1, ECS::RigidBody{{1.0f, 0.0f}});

    EntityID e2 = em.createEntity();
    em.addComponent(e2, ECS::Transform{{10.0f, 10.0f}});
    em.addComponent(e2, ECS::RigidBody{{0.0f, -2.0f}});

    pm.update(1.0f);

    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e1).position.x, 1.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e1).position.y, 0.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e2).position.x, 10.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e2).position.y, 8.0f);
}

TEST(PhysicsManagerTest, MultipleUpdatesAccumulate)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e, ECS::RigidBody{{2.0f, 1.0f}});

    pm.update(0.5f);
    pm.update(0.5f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 2.0f);
    EXPECT_FLOAT_EQ(t.position.y, 1.0f);
}

TEST(PhysicsManagerTest, NegativeVelocity)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{10.0f, 10.0f}});
    em.addComponent(e, ECS::RigidBody{{-3.0f, -4.0f}});

    pm.update(1.0f);

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 7.0f);
    EXPECT_FLOAT_EQ(t.position.y, 6.0f);
}

TEST(PhysicsManagerTest, EntityWithoutRigidBodyNotMoved)
{
    EntityManager em;
    PhysicsManager pm(&em);

    // Entity with only Transform (no RigidBody)
    EntityID e1 = em.createEntity();
    em.addComponent(e1, ECS::Transform{{5.0f, 5.0f}});

    // Entity with both Transform and RigidBody
    EntityID e2 = em.createEntity();
    em.addComponent(e2, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e2, ECS::RigidBody{{1.0f, 0.0f}});

    pm.update(1.0f);

    // e1 should not have moved
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e1).position.x, 5.0f);
    // e2 should have moved
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e2).position.x, 1.0f);
}

TEST(PhysicsManagerTest, SmallDeltaTimePrecision)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID e = em.createEntity();
    em.addComponent(e, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(e, ECS::RigidBody{{60.0f, 0.0f}});

    // Simulate 60 frames at ~16.67ms each
    for (int i = 0; i < 60; i++)
    {
        pm.update(1.0f / 60.0f);
    }

    auto &t = em.getComponent<ECS::Transform>(e);
    EXPECT_NEAR(t.position.x, 60.0f, 0.01f);
}
