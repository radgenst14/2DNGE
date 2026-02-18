#include <gtest/gtest.h>
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/components/Transform.h"
#include "engine/ecs/components/RigidBody.h"

TEST(EntityTest, AddAndGetComponent)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();

    ECS::Transform transform{{10.0f, 20.0f}, 45.0f, {2.0f, 2.0f}};
    registry.addComponent<ECS::Transform>(entity, transform);

    const auto &retrieved = registry.getComponent<ECS::Transform>(entity);
    EXPECT_FLOAT_EQ(retrieved.position.x, 10.0f);
    EXPECT_FLOAT_EQ(retrieved.position.y, 20.0f);
    EXPECT_FLOAT_EQ(retrieved.rotation, 45.0f);
    EXPECT_FLOAT_EQ(retrieved.scale.x, 2.0f);
    EXPECT_FLOAT_EQ(retrieved.scale.y, 2.0f);
}

TEST(EntityTest, HasComponentReturnsFalseWhenMissing)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();

    EXPECT_FALSE(registry.hasComponent<ECS::Transform>(entity));
}

TEST(EntityTest, HasComponentReturnsTrueWhenPresent)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();
    registry.addComponent<ECS::Transform>(entity, ECS::Transform{});

    EXPECT_TRUE(registry.hasComponent<ECS::Transform>(entity));
}

TEST(EntityTest, DefaultComponentValues)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();
    registry.addComponent<ECS::Transform>(entity, ECS::Transform{});

    const auto &t = registry.getComponent<ECS::Transform>(entity);
    EXPECT_FLOAT_EQ(t.position.x, 0.0f);
    EXPECT_FLOAT_EQ(t.position.y, 0.0f);
    EXPECT_FLOAT_EQ(t.rotation, 0.0f);
    EXPECT_FLOAT_EQ(t.scale.x, 1.0f);
    EXPECT_FLOAT_EQ(t.scale.y, 1.0f);
}

TEST(EntityTest, DefaultRigidBodyValues)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();
    registry.addComponent<ECS::RigidBody>(entity, ECS::RigidBody{});

    const auto &rb = registry.getComponent<ECS::RigidBody>(entity);
    EXPECT_FLOAT_EQ(rb.velocity.x, 0.0f);
    EXPECT_FLOAT_EQ(rb.velocity.y, 0.0f);
    EXPECT_FLOAT_EQ(rb.mass, 1.0f);
}

TEST(EntityTest, ModifyComponentInPlace)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();
    registry.addComponent<ECS::Transform>(entity, ECS::Transform{});

    auto &t = registry.getComponent<ECS::Transform>(entity);
    t.position = {50.0f, 75.0f};
    t.rotation = 180.0f;

    const auto &check = registry.getComponent<ECS::Transform>(entity);
    EXPECT_FLOAT_EQ(check.position.x, 50.0f);
    EXPECT_FLOAT_EQ(check.position.y, 75.0f);
    EXPECT_FLOAT_EQ(check.rotation, 180.0f);
}

TEST(EntityTest, MultipleComponentTypes)
{
    EntityManager registry;
    EntityID entity = registry.createEntity();

    registry.addComponent<ECS::Transform>(entity, ECS::Transform{{1.0f, 2.0f}});
    registry.addComponent<ECS::RigidBody>(entity, ECS::RigidBody{{3.0f, 4.0f}, 10.0f});

    EXPECT_TRUE(registry.hasComponent<ECS::Transform>(entity));
    EXPECT_TRUE(registry.hasComponent<ECS::RigidBody>(entity));

    EXPECT_FLOAT_EQ(registry.getComponent<ECS::Transform>(entity).position.x, 1.0f);
    EXPECT_FLOAT_EQ(registry.getComponent<ECS::RigidBody>(entity).velocity.x, 3.0f);
    EXPECT_FLOAT_EQ(registry.getComponent<ECS::RigidBody>(entity).mass, 10.0f);
}
