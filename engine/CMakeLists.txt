# Collect all engine source files
add_library(2dnge_engine
    core/Window.cpp
    core/Window.h
    core/Application.cpp
    core/Application.h
    core/Timer.cpp
    core/Timer.h
    renderer/Renderer.cpp
    renderer/Renderer.h
)

target_include_directories(2dnge_engine PUBLIC ${CMAKE_SOURCE_DIR})

# Link all dependencies
target_link_libraries(2dnge_engine PUBLIC
    SDL2::SDL2
    glm::glm
    enet
    imgui
)
