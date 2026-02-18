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

include(GoogleTest)

# Copy SDL2 DLL next to the test executable so it can be found at runtime
add_custom_command(TARGET 2dnge_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:2dnge_tests>
)

gtest_discover_tests(2dnge_tests)