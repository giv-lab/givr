#pragma once

#include <glm/glm.hpp>

#include "gl.h"

namespace givr {

    using vec2f = glm::vec2;
    using vec3f = glm::vec3;
    using mat4f = glm::mat4;

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
