cmake_minimum_required(VERSION 3.15)
project(2DNGE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Dependencies ─────────────────────────────────────

# OpenGL (needed by ImGui backend)
find_package(OpenGL REQUIRED)

include(FetchContent)

# GLM (header-only math library, fetched from source)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# SDL2 (fetched from source)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.12
)
FetchContent_MakeAvailable(SDL2)

# SDL2_image (fetched from source)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-2.8.4
)
FetchContent_MakeAvailable(SDL2_image)

# ENet (fetched from source)
FetchContent_Declare(
  enet
  GIT_REPOSITORY https://github.com/lsalzman/enet.git
  GIT_TAG v1.3.18
)
FetchContent_MakeAvailable(enet)

# Python3 (needed for embedding)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Derive the Python home from sys.base_prefix (works correctly with virtualenvs)
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import sys; print(sys.base_prefix, end='')"
    OUTPUT_VARIABLE PYTHON_HOME_DIR
)
# Convert backslashes to forward slashes so the C #define doesn't have escape issues
string(REPLACE "\\" "/" PYTHON_HOME_DIR "${PYTHON_HOME_DIR}")

# Generate a config header so the engine knows where Python lives at runtime
configure_file(
    ${CMAKE_SOURCE_DIR}/engine/scripting/ScriptingConfig.h.in
    ${CMAKE_BINARY_DIR}/generated/ScriptingConfig.h
)

# pybind11 (C++/Python bridge)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.13.6
)
FetchContent_MakeAvailable(pybind11)

# Dear ImGui (no CMake build — we compile it ourselves)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.8
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

# Create a library target for ImGui (SDL2 + OpenGL3 backend)
add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)

# ── Subdirectories ───────────────────────────────────
add_subdirectory(engine)
add_subdirectory(app)

enable_testing()
add_subdirectory(testing)

# ── Stub generation ─────────────────────────────────
set(BINDING_SOURCES
    ${CMAKE_SOURCE_DIR}/engine/scripting/bindings/ECSBindings.cpp
    ${CMAKE_SOURCE_DIR}/engine/scripting/bindings/PhysicsBindings.cpp
    ${CMAKE_SOURCE_DIR}/engine/scripting/bindings/RenderBindings.cpp
    ${CMAKE_SOURCE_DIR}/engine/scripting/bindings/InputBindings.cpp
)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/game/engine.pyi
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_SOURCE_DIR}/tools/generate_stubs.py
        --output ${CMAKE_SOURCE_DIR}/game/engine.pyi
        ${BINDING_SOURCES}
    DEPENDS
        ${BINDING_SOURCES}
        ${CMAKE_SOURCE_DIR}/tools/generate_stubs.py
    COMMENT "Generating engine.pyi type stubs"
    VERBATIM
)

add_custom_target(generate_stubs ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/game/engine.pyi
)
