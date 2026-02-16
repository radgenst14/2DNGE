# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_tests
    test_core/test_window.cpp
    test_core/test_timer.cpp
    test_core/test_application.cpp
    test_ecs/test_componentPool.cpp
    test_ecs/test_entity.cpp
    test_ecs/test_entityManager.cpp
)

target_link_libraries(run_tests
    2dnge_engine
    GTest::gtest_main
)

include(GoogleTest)

# Copy SDL2 DLL next to the test executable so it can be found at runtime
add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:run_tests>
)

gtest_discover_tests(run_tests)