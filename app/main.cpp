#include <engine/core/Application.h>
#include <engine/core/SceneManager.h>
#include <engine/scripting/ScriptableScene.h>
#include <ScriptingConfig.h>
#include <cstdio>
#include <string>

int main(int argc, char *argv[])
{
    Application app;

    // Set the initial scene to the Python-scripted MainScene
    std::string scenePath = std::string(PROJECT_ROOT) + "/game/scenes/MainScene.py";
    app.getSceneManager()->setScene(std::make_unique<ScriptableScene>(scenePath, app.getWindow()));
    app.run();
    return 0;
}
