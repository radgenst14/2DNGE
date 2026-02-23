#ifndef RENDERBINDINGS_H
#define RENDERBINDINGS_H

#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

void registerRenderBindings(py::module_ &m);

#endif