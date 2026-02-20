# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "test_*/*.cpp")

add_executable(2dnge_tests ${TEST_SOURCES})

target_link_libraries(2dnge_tests
    2dnge_game
    2dnge_engine
    GTest::gtest_main
)

# Provide the project root so tests can locate data files by absolute path
target_compile_definitions(2dnge_tests PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

include(GoogleTest)

# Copy all runtime DLLs (SDL2, Python, etc.) next to the test executable
add_custom_command(TARGET 2dnge_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:2dnge_tests>
        $<TARGET_FILE_DIR:2dnge_tests>
    COMMAND_EXPAND_LISTS
)

# Use PRE_TEST discovery so CMake doesn't try to run the executable at build
# time (before DLLs like SDL2 and Python are in place).
gtest_discover_tests(2dnge_tests DISCOVERY_MODE PRE_TEST)