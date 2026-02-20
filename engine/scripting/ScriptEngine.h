#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#pragma once

#include <pybind11/embed.h>
#include <SDL.h>

namespace py = pybind11;

class ScriptEngine
{
public:
    ScriptEngine();
    ~ScriptEngine();

    /* Delete copy constructor and assignment operator */
    ScriptEngine(const ScriptEngine &) = delete;
    ScriptEngine &operator=(const ScriptEngine &) = delete;

    bool isInitialized() const { return mInitialized; }

    py::object execute(const std::string &code);

    bool loadScript(const std::string &path);

    template <typename... Args>
    py::object callFunction(const std::string &name, Args &&...args)
    {
        try
        {
            py::object func = mGlobals[name.c_str()];
            return func(std::forward<Args>(args)...);
        }
        catch (const py::error_already_set &e)
        {
            SDL_Log("Failed to call function '%s': %s", name.c_str(), e.what());
            return py::none();
        }
    }

private:
    static int sRefCount; //< Number of live ScriptEngine instances
    bool mInitialized;    //< Indicates whether the Python interpreter was successfully initialized
    py::object mGlobals;  //< A dictionary to hold global variables and functions for script execution
};

#endif