#ifndef RENDERER_H
#define RENDERER_H

#pragma once

class Renderer
{
public:
    Renderer();
    ~Renderer();

    /// Delete copy constructor and assignment operator
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

private:
};

#endif