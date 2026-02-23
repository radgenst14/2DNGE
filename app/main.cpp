#include <engine/core/Application.h>
#include <engine/core/SceneManager.h>
#include <engine/scripting/ScriptableScene.h>
#include <ScriptingConfig.h>
#include <cstdio>
#include <string>

int main(int argc, char *argv[])
{
    fprintf(stderr, "=== 2DNGE starting ===\n");
    Application app;

    // Set the initial scene to the Python-scripted MainScene
    std::string scenePath = std::string(PROJECT_ROOT) + "/game/scenes/MainScene.py";
    app.getSceneManager()->setScene(std::make_unique<ScriptableScene>(scenePath, app.getWindow()));

    fprintf(stderr, "=== Application created, entering run() ===\n");
    app.run();
    fprintf(stderr, "=== run() returned ===\n");

    return 0;
}
