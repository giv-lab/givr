#pragma once

#include <glm/glm.hpp>

#include "gl.h"

namespace givr {

    using vec2f = glm::vec2;
    using vec3f = glm::vec3;
    using mat4f = glm::mat4;

    enum class PrimitiveType {
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

    GLenum getMode(PrimitiveType const &t);

    enum class BufferUsageType {
        STATIC_DRAW,
        DYNAMIC_DRAW
    };
    GLenum getBufferUsageType(BufferUsageType const &d);

};// end namespace givr
