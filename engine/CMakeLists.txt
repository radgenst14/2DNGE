# Collect all engine source files
add_library(2dnge_engine STATIC
    core/ecs/EntityManager.cpp
    core/ecs/EntityManager.h
    core/ecs/ComponentTypeID.h
    core/ecs/ComponentPool.h
    core/ecs/components/Collider.h
    core/ecs/components/RigidBody.h
    core/ecs/components/Sprite.h
    core/ecs/components/Transform.h
    core/Window.cpp
    core/Window.h
    core/Application.cpp
    core/Application.h
    core/IScene.h
    core/SceneManager.cpp
    core/SceneManager.h
    core/Timer.cpp
    core/Timer.h
    core/InputManager.cpp
    core/InputManager.h
    physics/Math.h
    physics/CollisionDetector.cpp
    physics/CollisionDetector.h
    physics/PhysicsManager.cpp
    physics/PhysicsManager.h
    renderer/Renderer.cpp
    renderer/Renderer.h
    renderer/RenderManager.cpp
    renderer/RenderManager.h
    renderer/AssetManager.cpp
    renderer/AssetManager.h
    renderer/helpers/SpriteRenderer.cpp
    renderer/helpers/SpriteRenderer.h
    scripting/EngineBindings.cpp
    scripting/EngineBindings.h
    scripting/ScriptableScene.cpp
    scripting/ScriptableScene.h
    scripting/ScriptEngine.cpp
    scripting/ScriptEngine.h
    scripting/bindings/AssetBindings.cpp
    scripting/bindings/AssetBindings.h
    scripting/bindings/ECSBindings.cpp
    scripting/bindings/ECSBindings.h
    scripting/bindings/PhysicsBindings.cpp
    scripting/bindings/PhysicsBindings.h
    scripting/bindings/RenderBindings.cpp
    scripting/bindings/RenderBindings.h
    vendor/cute_aseprite.cpp
    vendor/cute_aseprite.h
)

target_include_directories(2dnge_engine PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/generated
)

# Link all dependencies
target_link_libraries(2dnge_engine PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    glm::glm
    enet
    imgui
    pybind11::embed
    Python3::Python
)
