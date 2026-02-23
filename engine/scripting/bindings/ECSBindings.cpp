#include "ECSBindings.h"
#include "../../ecs/EntityManager.h"
#include "../../ecs/components/Transform.h"
#include "../../ecs/components/RigidBody.h"
#include "../EngineBindings.h"

void registerECSBindings(py::module_ &m)
{
      m.def("create_entity", []()
            { return EngineBindings::getEntityManager()->createEntity(); }, "Create a new entity and return its unique ID.");

      m.def("add_transform", [](EntityID entity, float x, float y)
            {
        ECS::Transform t{};
        t.position = {x, y};
        EngineBindings::getEntityManager()->addComponent(entity, t); }, "Add a Transform component to an entity.");

      m.def("add_rigidbody", [](EntityID entity, float vx, float vy, float mass)
            {
        ECS::RigidBody rb{};
        rb.velocity = {vx, vy};
        rb.mass = mass;
        EngineBindings::getEntityManager()->addComponent(entity, rb); }, "Add a RigidBody component to an entity.");

      m.def("get_position", [](EntityID entity) -> py::tuple
            {
        auto &t = EngineBindings::getEntityManager()->getComponent<ECS::Transform>(entity);
        return py::make_tuple(t.position.x, t.position.y); }, "Get the position of an entity's Transform component.");
}