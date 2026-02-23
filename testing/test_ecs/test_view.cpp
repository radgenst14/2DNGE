#include <gtest/gtest.h>
#include "engine/core/ecs/EntityManager.h"
#include "engine/core/ecs/components/Transform.h"
#include "engine/core/ecs/components/RigidBody.h"
#include "engine/core/ecs/components/Collider.h"
#include "engine/core/ecs/components/Sprite.h"

// ===========================================================================
// Single-component view
// ===========================================================================

TEST(ViewTest, SingleComponentViewIteratesAllMatching)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});
    // e2 has no Transform

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform>())
    {
        EXPECT_TRUE(em.hasComponent<ECS::Transform>(entity));
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(ViewTest, SingleComponentViewEmpty)
{
    EntityManager em;
    em.createEntity();
    em.createEntity();

    // No entities have Transform, but we need the pool to exist for view
    // If no pool exists, view should handle it gracefully with empty iteration
    int count = 0;
    for (EntityID entity : em.view<ECS::Transform>())
    {
        (void)entity;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(ViewTest, SingleComponentAllEntitiesMatch)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{3.0f, 0.0f}});

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform>())
    {
        (void)entity;
        count++;
    }
    EXPECT_EQ(count, 3);
}

// ===========================================================================
// Multi-component view
// ===========================================================================

TEST(ViewTest, TwoComponentViewMatchesBoth)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    // e0 has both
    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e0, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});

    // e1 has only Transform
    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});

    // e2 has only RigidBody
    em.addComponent<ECS::RigidBody>(e2, ECS::RigidBody{{3.0f, 0.0f}, 1.0f});

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        EXPECT_TRUE(em.hasComponent<ECS::Transform>(entity));
        EXPECT_TRUE(em.hasComponent<ECS::RigidBody>(entity));
        count++;
    }
    EXPECT_EQ(count, 1);
    // Only e0 has both components
}

TEST(ViewTest, TwoComponentViewNoneMatch)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();

    // e0 has only Transform
    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});

    // e1 has only RigidBody
    em.addComponent<ECS::RigidBody>(e1, ECS::RigidBody{{3.0f, 0.0f}, 1.0f});

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        (void)entity;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(ViewTest, TwoComponentViewAllMatch)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e0, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e1, ECS::RigidBody{{0.0f, 0.0f}, 2.0f});

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        (void)entity;
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(ViewTest, ThreeComponentView)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    // e0 has all three
    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e0, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});
    ECS::Collider c0;
    c0.type = ECS::ColliderType::Box;
    em.addComponent<ECS::Collider>(e0, c0);

    // e1 has Transform + RigidBody only
    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e1, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});

    // e2 has Transform + Collider only
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{3.0f, 0.0f}});
    ECS::Collider c2;
    c2.type = ECS::ColliderType::Circle;
    em.addComponent<ECS::Collider>(e2, c2);

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody, ECS::Collider>())
    {
        EXPECT_EQ(entity, e0);
        count++;
    }
    EXPECT_EQ(count, 1);
}

// ===========================================================================
// View correctly accesses components
// ===========================================================================

TEST(ViewTest, CanAccessComponentsInViewLoop)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{10.0f, 20.0f}});
    em.addComponent<ECS::RigidBody>(e0, ECS::RigidBody{{1.0f, 2.0f}, 5.0f});

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{30.0f, 40.0f}});
    em.addComponent<ECS::RigidBody>(e1, ECS::RigidBody{{3.0f, 4.0f}, 10.0f});

    float totalMass = 0.0f;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        const auto &rb = em.getComponent<ECS::RigidBody>(entity);
        totalMass += rb.mass;
    }
    EXPECT_FLOAT_EQ(totalMass, 15.0f);
}

TEST(ViewTest, CanModifyComponentsInViewLoop)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e0, ECS::RigidBody{{10.0f, 0.0f}, 1.0f});

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{0.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e1, ECS::RigidBody{{20.0f, 0.0f}, 1.0f});

    // Simulate a simple physics step: position += velocity * dt
    float dt = 0.1f;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        auto &t = em.getComponent<ECS::Transform>(entity);
        const auto &rb = em.getComponent<ECS::RigidBody>(entity);
        t.position.x += rb.velocity.x * dt;
    }

    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e0).position.x, 1.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e1).position.x, 2.0f);
}

// ===========================================================================
// View after entity deletion
// ===========================================================================

TEST(ViewTest, ViewAfterEntityDeletion)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{3.0f, 0.0f}});

    em.deleteEntity(e1);

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform>())
    {
        EXPECT_NE(entity, e1);
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(ViewTest, MultiComponentViewAfterDeletion)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e0, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{2.0f, 0.0f}});
    em.addComponent<ECS::RigidBody>(e1, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});

    em.deleteEntity(e0);

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        EXPECT_EQ(entity, e1);
        count++;
    }
    EXPECT_EQ(count, 1);
}

// ===========================================================================
// View picks smallest pool for iteration
// ===========================================================================

TEST(ViewTest, ViewUsesSmallestPoolForIteration)
{
    EntityManager em;

    // Create 100 entities with Transform
    for (int i = 0; i < 100; ++i)
    {
        EntityID e = em.createEntity();
        em.addComponent<ECS::Transform>(e, ECS::Transform{{static_cast<float>(i), 0.0f}});
    }

    // Only 2 of those entities get RigidBody
    em.addComponent<ECS::RigidBody>(0, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});
    em.addComponent<ECS::RigidBody>(50, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        EXPECT_TRUE(em.hasComponent<ECS::Transform>(entity));
        EXPECT_TRUE(em.hasComponent<ECS::RigidBody>(entity));
        count++;
    }
    EXPECT_EQ(count, 2);
}

// ===========================================================================
// View with Sprite component (new addition)
// ===========================================================================

TEST(ViewTest, SpriteAndTransformView)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    // e0: Sprite + Transform
    em.addComponent<ECS::Transform>(e0, ECS::Transform{{100.0f, 200.0f}});
    em.addComponent<ECS::Sprite>(e0, ECS::Sprite{"player", 32, 32});

    // e1: Transform only
    em.addComponent<ECS::Transform>(e1, ECS::Transform{{300.0f, 400.0f}});

    // e2: Sprite + Transform
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{500.0f, 600.0f}});
    em.addComponent<ECS::Sprite>(e2, ECS::Sprite{"enemy", 64, 64});

    int count = 0;
    for (EntityID entity : em.view<ECS::Sprite, ECS::Transform>())
    {
        const auto &sprite = em.getComponent<ECS::Sprite>(entity);
        const auto &transform = em.getComponent<ECS::Transform>(entity);
        EXPECT_TRUE(sprite.width > 0);
        EXPECT_TRUE(sprite.height > 0);
        EXPECT_TRUE(transform.position.x != 0.0f);
        count++;
    }
    EXPECT_EQ(count, 2);
}

// ===========================================================================
// Iterator correctness
// ===========================================================================

TEST(ViewTest, BeginEqualsEndWhenEmpty)
{
    EntityManager em;
    // Need at least a pool registered so view doesn't crash on missing pool
    em.createEntity(); // just create entity, no components

    auto v = em.view<ECS::Transform>();
    EXPECT_FALSE(v.begin() != v.end());
}

TEST(ViewTest, IteratorDereferencesCorrectEntity)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    em.addComponent<ECS::Transform>(e0, ECS::Transform{{7.0f, 8.0f}});

    auto v = em.view<ECS::Transform>();
    auto it = v.begin();
    EXPECT_EQ(*it, e0);
}

TEST(ViewTest, IteratorIncrementAdvances)
{
    EntityManager em;
    EntityID e0 = em.createEntity();
    EntityID e1 = em.createEntity();

    em.addComponent<ECS::Transform>(e0, ECS::Transform{});
    em.addComponent<ECS::Transform>(e1, ECS::Transform{});

    auto v = em.view<ECS::Transform>();
    auto it = v.begin();
    EntityID first = *it;
    ++it;
    EntityID second = *it;

    EXPECT_NE(first, second);
    ++it;
    EXPECT_FALSE(it != v.end());
}

// ===========================================================================
// Large-scale view
// ===========================================================================

TEST(ViewTest, LargeScaleView)
{
    EntityManager em;
    const int N = 1000;

    for (int i = 0; i < N; ++i)
    {
        EntityID e = em.createEntity();
        em.addComponent<ECS::Transform>(e, ECS::Transform{{static_cast<float>(i), 0.0f}});
        if (i % 2 == 0)
        {
            em.addComponent<ECS::RigidBody>(e, ECS::RigidBody{{0.0f, 0.0f}, 1.0f});
        }
    }

    int count = 0;
    for (EntityID entity : em.view<ECS::Transform, ECS::RigidBody>())
    {
        (void)entity;
        count++;
    }
    EXPECT_EQ(count, 500);
}
