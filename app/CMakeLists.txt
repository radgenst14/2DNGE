add_executable(2dnge_app main.cpp)
target_link_libraries(2dnge_app PRIVATE 2dnge_engine SDL2::SDL2main)

# Copy SDL2 DLL next to the app executable so it can be found at runtime
add_custom_command(TARGET 2dnge_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:2dnge_app>
)
