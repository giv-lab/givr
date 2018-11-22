#pragma once

#include "givm.h"

#include <glad/glad.h>

using vec3f = givm::vec3f;
using mat4f = givm::mat4f;

namespace givr {
    enum class primitive_type {
        POINTS,
        LINES,
        LINE_LOOP,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        LINES_ADJACENCY,
        LINE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY,
        TRIANGLE_STRIP_ADJACENCY

    };

    GLenum get_mode(primitive_type const &t);

    enum class buffer_usage_type {
        STATIC_DRAW,
        DYNAMIC_DRAW
    };
    GLenum get_buffer_usage_type(buffer_usage_type const &d);

};// end namespace givr
