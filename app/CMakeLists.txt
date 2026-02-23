add_executable(2dnge_app main.cpp)
target_link_libraries(2dnge_app PRIVATE 2dnge_engine SDL2::SDL2main)

# Copy runtime DLLs next to the app executable so they can be found at runtime
add_custom_command(TARGET 2dnge_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:2dnge_app>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_image::SDL2_image>
        $<TARGET_FILE_DIR:2dnge_app>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Python3::Python>
        $<TARGET_FILE_DIR:2dnge_app>
)
