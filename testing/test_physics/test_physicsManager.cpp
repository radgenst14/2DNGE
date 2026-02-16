#include <gtest/gtest.h>

#include "engine/ecs/components/Transform.h"
#include "engine/ecs/components/RigidBody.h"
#include "engine/ecs/components/Collider.h"
#include "engine/physics/PhysicsManager.h"
#include "engine/ecs/EntityManager.h"

TEST(PhysicsManagerTest, UpdateMovesEntitiesWithRigidBody)
{
    EntityManager entityManager;
    PhysicsManager physicsManager(&entityManager);

    // Create an entity with Transform and RigidBody components
    EntityID entity = entityManager.createEntity();
    entityManager.addComponent(entity, ECS::Transform{{0.0f, 0.0f}});
    entityManager.addComponent(entity, ECS::RigidBody{{1.0f, 0.0f}}); // Velocity of 1 unit/sec to the right

    // Update physics for 1 second
    physicsManager.update(1.0f);

    // Check that the entity's position has been updated correctly
    auto &transform = entityManager.getComponent<ECS::Transform>(entity);
    EXPECT_FLOAT_EQ(transform.position.x, 1.0f); // Should have moved 1 unit to the right
    EXPECT_FLOAT_EQ(transform.position.y, 0.0f); // Should not have moved vertically
}