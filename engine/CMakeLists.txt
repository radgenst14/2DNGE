# Collect all engine source files
add_library(2dnge_engine
    core/Window.cpp
    core/Window.h
    core/Application.cpp
    core/Application.h
)

target_include_directories(2dnge_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link all dependencies
target_link_libraries(2dnge_engine PUBLIC
    SDL2::SDL2
    glm::glm
    enet
    imgui
    Python3::Python
)
