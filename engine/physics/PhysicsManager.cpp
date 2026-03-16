#include "PhysicsManager.h"
#include "CollisionDetector.h"
#include "CollisionHandler.h"
#include "../core/ecs/EntityManager.h"
#include "../core/ecs/components/Transform.h"
#include "../core/ecs/components/RigidBody.h"
#include "../core/ecs/components/Collider.h"

PhysicsManager::PhysicsManager(EntityManager *entityManager)
    : mEntityManager(entityManager),
      mCollisionDetector(std::make_unique<CollisionDetector>(entityManager)),
      mCollisionHandler(std::make_unique<CollisionHandler>(entityManager))
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::update(float dt)
{
    // Transform entities with mobile components : RigidBody
    auto &pool = mEntityManager->getComponentPool<ECS::RigidBody>();

    for (auto &entry : pool.getDenseToEntity())
    {
        EntityID entity = entry;
        auto &rigidBody = pool.get(entity);
        auto &transform = mEntityManager->getComponent<ECS::Transform>(entity);

        // Update position based on velocity
        auto displacement = rigidBody.velocity * dt;
        transform.position += displacement;
    }

    // Detect and resolve collisions
    auto &colliderPool = mEntityManager->getComponentPool<ECS::Collider>();
    const auto &entities = colliderPool.getDenseToEntity();

    for (size_t i = 0; i < entities.size(); ++i)
    {
        for (size_t j = i + 1; j < entities.size(); ++j)
        {
            auto result = mCollisionDetector->checkCollision(entities[i], entities[j]);

            if (result.isColliding)
            {
                mCollisionHandler->resolveCollision(entities[i], entities[j], result);
            }
        }
    }
}