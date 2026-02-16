add_library(2dnge_game
    scenes/PhysicsTestScene.cpp
)
target_link_libraries(2dnge_game PUBLIC 2dnge_engine)
target_include_directories(2dnge_game PUBLIC ${CMAKE_SOURCE_DIR})