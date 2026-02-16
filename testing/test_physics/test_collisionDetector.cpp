#include <gtest/gtest.h>

#include "engine/ecs/components/Collider.h"
#include "engine/ecs/components/Transform.h"
#include "engine/physics/CollisionDetector.h"
#include "engine/ecs/EntityManager.h"

TEST(CollisionDetectorTest, BoxBoxCollision)
{
    EntityManager entityManager;
    CollisionDetector collisionDetector(&entityManager);

    // Create two entities with box colliders
    EntityID entityA = entityManager.createEntity();
    EntityID entityB = entityManager.createEntity();

    // Add Transform and Collider components to both entities
    entityManager.addComponent<ECS::Transform>(entityA, ECS::Transform{});
    entityManager.addComponent<ECS::Collider>(entityA, ECS::Collider{});
    entityManager.addComponent<ECS::Transform>(entityB, ECS::Transform{});
    entityManager.addComponent<ECS::Collider>(entityB, ECS::Collider{});

    // Check for collision - they should collide
    EXPECT_TRUE(collisionDetector.checkCollision(entityA, entityB));
}

TEST(CollisionDetectorTest, CircleCircleCollision)
{
    EntityManager entityManager;
    CollisionDetector collisionDetector(&entityManager);

    // Create two entities with circle colliders
    EntityID entityA = entityManager.createEntity();
    EntityID entityB = entityManager.createEntity();

    // Add Transform and Collider components to both entities
    ECS::Transform transformA;
    transformA.position = glm::vec2(0.0f, 0.0f);
    entityManager.addComponent<ECS::Transform>(entityA, transformA);

    ECS::Collider colliderA;
    colliderA.type = ECS::ColliderType::Circle;
    colliderA.radius = 1.0f;
    entityManager.addComponent<ECS::Collider>(entityA, colliderA);

    ECS::Transform transformB;
    transformB.position = glm::vec2(1.5f, 0.0f); // Positioned to just touch the first circle
    entityManager.addComponent<ECS::Transform>(entityB, transformB);

    ECS::Collider colliderB;
    colliderB.type = ECS::ColliderType::Circle;
    colliderB.radius = 1.0f;
    entityManager.addComponent<ECS::Collider>(entityB, colliderB);

    // Check for collision - they should collide
    EXPECT_TRUE(collisionDetector.checkCollision(entityA, entityB));
}

TEST(CollisionDetectorTest, BoxCircleCollision)
{
    EntityManager entityManager;
    CollisionDetector collisionDetector(&entityManager);

    // Create an entity with a box collider and another with a circle collider
    EntityID entityA = entityManager.createEntity();
    EntityID entityB = entityManager.createEntity();

    // Add Transform and Collider components to both entities
    ECS::Transform transformA;
    transformA.position = glm::vec2(0.0f, 0.0f);
    entityManager.addComponent<ECS::Transform>(entityA, transformA);

    ECS::Collider colliderA;
    colliderA.type = ECS::ColliderType::Box;
    colliderA.size = glm::vec2(2.0f, 2.0f);
    entityManager.addComponent<ECS::Collider>(entityA, colliderA);

    ECS::Transform transformB;
    transformB.position = glm::vec2(1.5f, 0.0f); // Positioned to just touch the box
    entityManager.addComponent<ECS::Transform>(entityB, transformB);

    ECS::Collider colliderB;
    colliderB.type = ECS::ColliderType::Circle;
    colliderB.radius = 1.0f;
    entityManager.addComponent<ECS::Collider>(entityB, colliderB);

    // Check for collision - they should collide
    EXPECT_TRUE(collisionDetector.checkCollision(entityA, entityB));
}