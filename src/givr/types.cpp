#include "types.h"

using namespace givr;

GLenum givr::get_mode(primitive_type const &t) {
    switch(t) {
        case primitive_type::POINTS:
            return GL_POINTS;
        case primitive_type::LINES:
            return GL_LINES;
        case primitive_type::LINE_LOOP:
            return GL_LINE_LOOP;
        case primitive_type::LINE_STRIP:
            return GL_LINE_STRIP;
        case primitive_type::TRIANGLES:
            return GL_TRIANGLES;
        case primitive_type::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case primitive_type::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case primitive_type::LINES_ADJACENCY:
            return GL_LINES_ADJACENCY;
        case primitive_type::LINE_STRIP_ADJACENCY:
            return GL_LINE_STRIP_ADJACENCY;
        case primitive_type::TRIANGLES_ADJACENCY:
            return GL_TRIANGLES_ADJACENCY;
        case primitive_type::TRIANGLE_STRIP_ADJACENCY:
            return GL_TRIANGLE_STRIP_ADJACENCY;
        default:
            return GL_TRIANGLES;
    }
}

GLenum givr::get_buffer_usage_type(buffer_usage_type const &d) {
    switch(d) {
        case buffer_usage_type::STATIC_DRAW:
            return GL_STATIC_DRAW;
        case buffer_usage_type::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        default:
            return GL_STATIC_DRAW;
    }

}
