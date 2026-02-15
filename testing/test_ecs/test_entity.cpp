#include <gtest/gtest.h>
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/components/Transform.h"

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
