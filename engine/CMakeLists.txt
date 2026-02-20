# Collect all engine source files
add_library(2dnge_engine
    ecs/EntityManager.cpp
    ecs/EntityManager.h
    ecs/ComponentTypeID.h
    ecs/ComponentPool.h
    ecs/components/Collider.h
    ecs/components/Transform.h
    ecs/components/RigidBody.h
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
    scripting/ScriptEngine.cpp
    scripting/ScriptEngine.h
)

target_include_directories(2dnge_engine PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/generated
)

# Link all dependencies
target_link_libraries(2dnge_engine PUBLIC
    SDL2::SDL2
    glm::glm
    enet
    imgui
    pybind11::embed
    Python3::Python
)
