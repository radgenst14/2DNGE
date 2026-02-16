#include "PhysicsManager.h"
#include "../ecs/EntityManager.h"
#include "../ecs/components/Transform.h"
#include "../ecs/components/RigidBody.h"
#include "../ecs/components/Collider.h"

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
}