cmake_minimum_required(VERSION 3.15)
project(2DNGE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Dependencies ─────────────────────────────────────

# SDL2 (install via system package manager, vcpkg, or manually)
find_package(SDL2 REQUIRED)

# GLM (header-only math library)
find_package(glm REQUIRED)

# Python3 (for scripting)
find_package(Python3 REQUIRED COMPONENTS Development)

# OpenGL (needed by ImGui backend)
find_package(OpenGL REQUIRED)

# ENet (fetched from source)
include(FetchContent)
FetchContent_Declare(
  enet
  GIT_REPOSITORY https://github.com/lsalzman/enet.git
  GIT_TAG v1.3.18
)
FetchContent_MakeAvailable(enet)

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
add_subdirectory(testing)
