#include "AssetBindings.h"
#include "../EngineBindings.h"
#include "../../renderer/AssetManager.h"
#include <ScriptingConfig.h>

void registerAssetBindings(py::module_ &m)
{
    m.attr("PROJECT_ROOT") = PROJECT_ROOT;

    m.def("load_texture", [](const std::string &id, const std::string &path)
          {
        auto assetManager = EngineBindings::getAssetManager();
        if (!assetManager->loadTexture(id, path))
        {
            throw std::runtime_error("Failed to load texture: " + path);
        } }, py::arg("id"), py::arg("path"), "Load a texture from a file and store it with a unique ID.");

    m.def("load_aseprite", [](const std::string &id, const std::string &path)
          {
        auto assetManager = EngineBindings::getAssetManager();
        if (!assetManager->loadAseprite(id, path))
        {
            throw std::runtime_error("Failed to load aseprite: " + path);
        } }, py::arg("id"), py::arg("path"), "Load an .aseprite file and store it as a texture with a unique ID.");
}