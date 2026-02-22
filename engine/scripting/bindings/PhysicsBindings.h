#ifndef PHYSICSBINDINGS_H
#define PHYSICSBINDINGS_H

#pragma once

#include <pybind11/embed.h>

namespace py = pybind11;

void registerPhysicsBindings(py::module_ &m);

#endif