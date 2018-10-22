#pragma once

#include "givm.h"

using vec3f = givm::vec3f;
using mat4f = givm::mat4f;

namespace givr {
    enum class primitive_type {
        POINTS,
        LINES,
        TRIANGLES
    };

};// end namespace givr
