#include <gtest/gtest.h>

#include "engine/core/ecs/components/Transform.h"
#include "engine/core/ecs/components/RigidBody.h"
#include "engine/core/ecs/components/Collider.h"
#include "engine/physics/PhysicsManager.h"
#include "engine/core/ecs/EntityManager.h"

// =============================================================================
// Velocity integration (existing tests, preserved)
// =============================================================================

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

// =============================================================================
// Collision detection + resolution integration
// =============================================================================

TEST(PhysicsManagerTest, CollidingEntitiesGetSeparated)
{
    EntityManager em;
    PhysicsManager pm(&em);

    // Two overlapping boxes with colliders, moving toward each other
    EntityID a = em.createEntity();
    em.addComponent(a, ECS::Transform{{-0.5f, 0.0f}});
    em.addComponent(a, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});
    ECS::Collider ca;
    ca.type = ECS::ColliderType::Box;
    ca.size = {2.0f, 2.0f};
    em.addComponent(a, ca);

    EntityID b = em.createEntity();
    em.addComponent(b, ECS::Transform{{0.5f, 0.0f}});
    em.addComponent(b, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});
    ECS::Collider cb;
    cb.type = ECS::ColliderType::Box;
    cb.size = {2.0f, 2.0f};
    em.addComponent(b, cb);

    // They overlap: A AABB [-1.5,-1] to [0.5,1], B AABB [-0.5,-1] to [1.5,1]
    pm.update(0.0f); // zero dt so no velocity integration, just collision

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // After resolution, they should be pushed apart
    EXPECT_LT(tA.position.x, tB.position.x);
    // The gap should be >= 2.0 (their combined half-widths)
    // Check they're no longer overlapping
    float aRight = tA.position.x + 1.0f;
    float bLeft = tB.position.x - 1.0f;
    EXPECT_LE(aRight, bLeft + 0.01f); // allow small epsilon
}

TEST(PhysicsManagerTest, NonCollidingEntitiesUnaffected)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID a = em.createEntity();
    em.addComponent(a, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(a, ECS::RigidBody{{1.0f, 0.0f}, 1.0f});
    ECS::Collider ca;
    ca.type = ECS::ColliderType::Box;
    ca.size = {1.0f, 1.0f};
    em.addComponent(a, ca);

    EntityID b = em.createEntity();
    em.addComponent(b, ECS::Transform{{10.0f, 0.0f}});
    em.addComponent(b, ECS::RigidBody{{-1.0f, 0.0f}, 1.0f});
    ECS::Collider cb;
    cb.type = ECS::ColliderType::Box;
    cb.size = {1.0f, 1.0f};
    em.addComponent(b, cb);

    pm.update(1.0f);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // Just velocity integration, no collision
    EXPECT_FLOAT_EQ(tA.position.x, 1.0f);
    EXPECT_FLOAT_EQ(tB.position.x, 9.0f);
}

TEST(PhysicsManagerTest, HeadOnCollisionReversesVelocities)
{
    EntityManager em;
    PhysicsManager pm(&em);

    // Two objects approaching head-on with perfect bounce
    EntityID a = em.createEntity();
    ECS::Transform ta;
    ta.position = {-0.5f, 0.0f};
    em.addComponent(a, ta);
    ECS::RigidBody rbA;
    rbA.velocity = {1.0f, 0.0f};
    rbA.mass = 1.0f;
    rbA.restitution = 1.0f;
    em.addComponent(a, rbA);
    ECS::Collider ca;
    ca.type = ECS::ColliderType::Box;
    ca.size = {2.0f, 2.0f};
    em.addComponent(a, ca);

    EntityID b = em.createEntity();
    ECS::Transform tb;
    tb.position = {0.5f, 0.0f};
    em.addComponent(b, tb);
    ECS::RigidBody rbB;
    rbB.velocity = {-1.0f, 0.0f};
    rbB.mass = 1.0f;
    rbB.restitution = 1.0f;
    em.addComponent(b, rbB);
    ECS::Collider cb;
    cb.type = ECS::ColliderType::Box;
    cb.size = {2.0f, 2.0f};
    em.addComponent(b, cb);

    // dt=0 to isolate collision resolution from movement
    pm.update(0.0f);

    auto &finalRbA = em.getComponent<ECS::RigidBody>(a);
    auto &finalRbB = em.getComponent<ECS::RigidBody>(b);

    // With restitution=1.0 and equal mass, velocities should swap
    EXPECT_NEAR(finalRbA.velocity.x, -1.0f, 0.01f);
    EXPECT_NEAR(finalRbB.velocity.x, 1.0f, 0.01f);
}

TEST(PhysicsManagerTest, EntitiesWithoutCollidersSkipCollision)
{
    EntityManager em;
    PhysicsManager pm(&em);

    // Two overlapping entities but NO colliders -> no collision resolution
    EntityID a = em.createEntity();
    em.addComponent(a, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(a, ECS::RigidBody{{1.0f, 0.0f}});

    EntityID b = em.createEntity();
    em.addComponent(b, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent(b, ECS::RigidBody{{-1.0f, 0.0f}});

    pm.update(1.0f);

    // Just velocity integration, no collision (no colliders)
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(a).position.x, 1.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(b).position.x, -1.0f);
}

TEST(PhysicsManagerTest, CircleCollisionInUpdateLoop)
{
    EntityManager em;
    PhysicsManager pm(&em);

    EntityID a = em.createEntity();
    em.addComponent(a, ECS::Transform{{0.0f, 0.0f}});
    ECS::RigidBody rbA;
    rbA.velocity = {0.0f, 0.0f};
    rbA.mass = 1.0f;
    em.addComponent(a, rbA);
    ECS::Collider ca;
    ca.type = ECS::ColliderType::Circle;
    ca.radius = 1.0f;
    em.addComponent(a, ca);

    EntityID b = em.createEntity();
    em.addComponent(b, ECS::Transform{{0.5f, 0.0f}});
    ECS::RigidBody rbB;
    rbB.velocity = {0.0f, 0.0f};
    rbB.mass = 1.0f;
    em.addComponent(b, rbB);
    ECS::Collider cb;
    cb.type = ECS::ColliderType::Circle;
    cb.radius = 1.0f;
    em.addComponent(b, cb);

    pm.update(0.0f);

    auto &tA = em.getComponent<ECS::Transform>(a);
    auto &tB = em.getComponent<ECS::Transform>(b);

    // Circles should be pushed apart
    EXPECT_LT(tA.position.x, tB.position.x);
    float distance = tB.position.x - tA.position.x;
    EXPECT_GE(distance, 1.99f); // should be ~2.0 (sum of radii)
}
