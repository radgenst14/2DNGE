#ifndef ECSBINDINGS_H
#define ECSBINDINGS_H

#pragma once

#include <pybind11/embed.h>

namespace py = pybind11;

void registerECSBindings(py::module_ &m);

#endif