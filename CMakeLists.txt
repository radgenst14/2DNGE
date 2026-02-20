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

# ENet (fetched from source)
FetchContent_Declare(
  enet
  GIT_REPOSITORY https://github.com/lsalzman/enet.git
  GIT_TAG v1.3.18
)
FetchContent_MakeAvailable(enet)

# Python3 (needed for embedding)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Derive the Python home (prefix) from the real executable path
get_target_property(_Python3_LOCATION Python3::Interpreter IMPORTED_LOCATION)
get_filename_component(PYTHON_HOME_DIR "${_Python3_LOCATION}" DIRECTORY)

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
add_subdirectory(game)
add_subdirectory(app)

enable_testing()
add_subdirectory(testing)
