#ifndef ASSETBINDINGS_H
#define ASSETBINDINGS_H

#pragma once

#include <pybind11/embed.h>

namespace py = pybind11;

void registerAssetBindings(py::module_ &m);

#endif