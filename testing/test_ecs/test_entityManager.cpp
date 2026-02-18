#include <gtest/gtest.h>
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/components/Transform.h"
#include "engine/ecs/components/RigidBody.h"
#include "engine/ecs/components/Collider.h"

TEST(EntityManagerTest, CreateEntityReturnsUniqueIDs)
{
    EntityManager em;
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();
    EntityID e3 = em.createEntity();

    EXPECT_NE(e1, e2);
    EXPECT_NE(e2, e3);
    EXPECT_NE(e1, e3);
}

TEST(EntityManagerTest, CreateEntityReturnsSequentialIDs)
{
    EntityManager em;
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    EXPECT_EQ(e2, e1 + 1);
}

TEST(EntityManagerTest, AddAndGetComponent)
{
    EntityManager em;
    EntityID e = em.createEntity();

    ECS::Transform t{{5.0f, 10.0f}, 90.0f, {2.0f, 2.0f}};
    em.addComponent<ECS::Transform>(e, t);

    const auto &retrieved = em.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(retrieved.position.x, 5.0f);
    EXPECT_FLOAT_EQ(retrieved.position.y, 10.0f);
    EXPECT_FLOAT_EQ(retrieved.rotation, 90.0f);
}

TEST(EntityManagerTest, HasComponentReturnsTrue)
{
    EntityManager em;
    EntityID e = em.createEntity();
    em.addComponent<ECS::Transform>(e, ECS::Transform{});

    EXPECT_TRUE(em.hasComponent<ECS::Transform>(e));
}

TEST(EntityManagerTest, HasComponentReturnsFalse)
{
    EntityManager em;
    EntityID e = em.createEntity();

    EXPECT_FALSE(em.hasComponent<ECS::Transform>(e));
}

TEST(EntityManagerTest, HasComponentReturnsFalseForDifferentType)
{
    EntityManager em;
    EntityID e = em.createEntity();
    em.addComponent<ECS::Transform>(e, ECS::Transform{});

    EXPECT_FALSE(em.hasComponent<ECS::RigidBody>(e));
}

TEST(EntityManagerTest, MultipleComponentsOnOneEntity)
{
    EntityManager em;
    EntityID e = em.createEntity();

    em.addComponent<ECS::Transform>(e, ECS::Transform{{1.0f, 2.0f}});
    em.addComponent<ECS::RigidBody>(e, ECS::RigidBody{{3.0f, 4.0f}, 5.0f});

    EXPECT_TRUE(em.hasComponent<ECS::Transform>(e));
    EXPECT_TRUE(em.hasComponent<ECS::RigidBody>(e));

    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e).position.x, 1.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::RigidBody>(e).velocity.x, 3.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::RigidBody>(e).mass, 5.0f);
}

TEST(EntityManagerTest, MultipleEntitiesSameComponent)
{
    EntityManager em;
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{2.0f, 0.0f}});

    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e1).position.x, 1.0f);
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e2).position.x, 2.0f);
}

TEST(EntityManagerTest, ModifyComponentThroughReference)
{
    EntityManager em;
    EntityID e = em.createEntity();
    em.addComponent<ECS::Transform>(e, ECS::Transform{{0.0f, 0.0f}});

    auto &t = em.getComponent<ECS::Transform>(e);
    t.position.x = 100.0f;

    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e).position.x, 100.0f);
}

TEST(EntityManagerTest, DeleteEntityRemovesAllComponents)
{
    EntityManager em;
    EntityID e = em.createEntity();
    em.addComponent<ECS::Transform>(e, ECS::Transform{});
    em.addComponent<ECS::RigidBody>(e, ECS::RigidBody{});

    em.deleteEntity(e);

    EXPECT_FALSE(em.hasComponent<ECS::Transform>(e));
    EXPECT_FALSE(em.hasComponent<ECS::RigidBody>(e));
}

TEST(EntityManagerTest, DeleteEntityDoesNotAffectOthers)
{
    EntityManager em;
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{2.0f, 0.0f}});

    em.deleteEntity(e1);

    EXPECT_FALSE(em.hasComponent<ECS::Transform>(e1));
    EXPECT_TRUE(em.hasComponent<ECS::Transform>(e2));
    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e2).position.x, 2.0f);
}

TEST(EntityManagerTest, GetComponentPoolIteration)
{
    EntityManager em;
    EntityID e1 = em.createEntity();
    EntityID e2 = em.createEntity();
    EntityID e3 = em.createEntity();

    em.addComponent<ECS::Transform>(e1, ECS::Transform{{1.0f, 0.0f}});
    em.addComponent<ECS::Transform>(e2, ECS::Transform{{2.0f, 0.0f}});
    // e3 has no Transform

    auto &pool = em.getComponentPool<ECS::Transform>();
    int count = 0;
    for (auto entity : pool)
    {
        EXPECT_TRUE(em.hasComponent<ECS::Transform>(entity));
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(EntityManagerTest, AddComponentReturnsReference)
{
    EntityManager em;
    EntityID e = em.createEntity();

    auto &ref = em.addComponent<ECS::Transform>(e, ECS::Transform{{5.0f, 5.0f}});
    ref.position.x = 42.0f;

    EXPECT_FLOAT_EQ(em.getComponent<ECS::Transform>(e).position.x, 42.0f);
}

TEST(EntityManagerTest, ConstGetComponent)
{
    EntityManager em;
    EntityID e = em.createEntity();
    em.addComponent<ECS::Transform>(e, ECS::Transform{{3.0f, 4.0f}});

    const EntityManager &constEm = em;
    const auto &t = constEm.getComponent<ECS::Transform>(e);
    EXPECT_FLOAT_EQ(t.position.x, 3.0f);
}

TEST(EntityManagerTest, ColliderComponent)
{
    EntityManager em;
    EntityID e = em.createEntity();

    ECS::Collider c;
    c.type = ECS::ColliderType::Circle;
    c.radius = 2.5f;
    c.isTrigger = true;
    em.addComponent<ECS::Collider>(e, c);

    const auto &retrieved = em.getComponent<ECS::Collider>(e);
    EXPECT_EQ(retrieved.type, ECS::ColliderType::Circle);
    EXPECT_FLOAT_EQ(retrieved.radius, 2.5f);
    EXPECT_TRUE(retrieved.isTrigger);
}
