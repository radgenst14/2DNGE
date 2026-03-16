#include "CollisionHandler.h"
#include "../core/ecs/EntityManager.h"
#include "../core/ecs/components/Transform.h"
#include "../core/ecs/components/RigidBody.h"

CollisionHandler::~CollisionHandler()
{
}

void CollisionHandler::resolveCollision(EntityID entityA, EntityID entityB, const Math::CollisionResult &result)
{
    auto &transformA = mEntityManager->getComponent<ECS::Transform>(entityA);
    auto &transformB = mEntityManager->getComponent<ECS::Transform>(entityB);

    bool aHasRB = mEntityManager->hasComponent<ECS::RigidBody>(entityA);
    bool bHasRB = mEntityManager->hasComponent<ECS::RigidBody>(entityB);

    // If neither has a rigidbody, nothing to resolve
    if (!aHasRB && !bHasRB)
        return;

    float inverseMassA = 0.0f;
    float inverseMassB = 0.0f;

    if (aHasRB)
        inverseMassA = 1.0f / mEntityManager->getComponent<ECS::RigidBody>(entityA).mass;
    if (bHasRB)
        inverseMassB = 1.0f / mEntityManager->getComponent<ECS::RigidBody>(entityB).mass;

    float totalInverseMass = inverseMassA + inverseMassB;

    // --- Positional correction ---
    transformA.position += result.normal * (result.penetration * inverseMassA / totalInverseMass);
    transformB.position -= result.normal * (result.penetration * inverseMassB / totalInverseMass);

    // --- Velocity response ---
    if (!aHasRB || !bHasRB)
        return; // Only do impulse if both have rigidbodies

    auto &rbA = mEntityManager->getComponent<ECS::RigidBody>(entityA);
    auto &rbB = mEntityManager->getComponent<ECS::RigidBody>(entityB);

    glm::vec2 relativeVelocity = rbA.velocity - rbB.velocity;
    float normalVelocity = glm::dot(relativeVelocity, result.normal);

    // If they're already separating, don't apply impulse
    if (normalVelocity > 0.0f)
        return;

    // Use the lower restitution of the two
    float restitution = std::min(rbA.restitution, rbB.restitution);

    float impulseMagnitude = -(1.0f + restitution) * normalVelocity / totalInverseMass;

    rbA.velocity += result.normal * (impulseMagnitude * inverseMassA);
    rbB.velocity -= result.normal * (impulseMagnitude * inverseMassB);
}