#include "ECSBindings.h"
#include "../EngineBindings.h"
#include "../../core/ecs/EntityManager.h"
#include "../../core/ecs/components/Transform.h"
#include "../../core/ecs/components/RigidBody.h"
#include "../../core/ecs/components/Sprite.h"
#include "../../renderer/AssetManager.h"

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

      m.def("add_sprite", [](EntityID entity, const std::string &textureId, int width, int height)
            {
        auto *assetManager = EngineBindings::getAssetManager();
        if (!assetManager || !assetManager->hasTexture(textureId))
        {
            throw std::runtime_error("Texture not found: '" + textureId + "'. Load it first with load_texture().");
        }

        if (width == 0 || height == 0)
        {
            assetManager->getTextureDimensions(textureId, width, height);
        }

        ECS::Sprite sprite{};
        sprite.textureId = textureId;
        sprite.width = width;
        sprite.height = height;
        EngineBindings::getEntityManager()->addComponent(entity, sprite); }, "Add a Sprite component to an entity. Width/height auto-filled from texture if omitted.", py::arg("entity"), py::arg("texture_id"), py::arg("width") = 0, py::arg("height") = 0);
}