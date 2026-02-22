#ifndef INPUTBINDINGS_H
#define INPUTBINDINGS_H

#pragma once

#include <pybind11/embed.h>

namespace py = pybind11;

void registerInputBindings(py::module_ &m);

#endif