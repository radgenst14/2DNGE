#include <engine/renderer/Renderer.h>
#include <engine/ecs/EntityManager.h>
#include <engine/ecs/ComponentTypeID.h>
#include <engine/ecs/components/Transform.h>
#include <engine/ecs/components/RigidBody.h>
#include <engine/physics/PhysicsManager.h>
#include "PhysicsTestScene.h"

void PhysicsTestScene::init()
{
    // Create the EntityManager and PhysicsManager for this scene
    mEntityManager = std::make_unique<EntityManager>();
    if (!mEntityManager)
    {
        SDL_Log("Failed to create EntityManager");
        return;
    }

    mPhysicsManager = std::make_unique<PhysicsManager>(mEntityManager.get());
    if (!mPhysicsManager)
    {
        SDL_Log("Failed to create PhysicsManager");
        return;
    }

    // Create test entities
    EntityID entity1 = mEntityManager->createEntity();

    // Add components to the entity (e.g., Transform, RigidBody)
    ECS::Transform transform{};
    transform.position = {100.0f, 100.0f};
    mEntityManager->addComponent(entity1, transform);

    // Add a RigidBody component with physics properties
    ECS::RigidBody rigidBody{};
    rigidBody.velocity = {50.0f, 0.0f}; // Initial velocity to the right
    rigidBody.mass = 1.0f;              // Mass of the entity
    mEntityManager->addComponent(entity1, rigidBody);
}

void PhysicsTestScene::input(SDL_Event &event)
{
}

void PhysicsTestScene::update(float dt)
{
    // Update the physics manager to apply physics calculations
    mPhysicsManager->update(dt);
}

void PhysicsTestScene::render(Renderer &renderer)
{
    renderer.setDrawColor(255, 255, 255, 255); // Set draw color to white
    // Render entities with a Transform component
    for (EntityID entity : mEntityManager->getComponentPool<ECS::Transform>())
    {
        const auto &transform = mEntityManager->getComponent<ECS::Transform>(entity);
        renderer.drawRect(transform, 50, 50); // Draw a rectangle representing the entity
    }
}

void PhysicsTestScene::cleanup()
{
}
