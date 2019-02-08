#include "types.h"

using PrimitiveType = givr::PrimitiveType;
using BufferUsageType = givr::BufferUsageType;

GLenum givr::getMode(PrimitiveType const &t) {
    switch(t) {
        case PrimitiveType::POINTS:
            return GL_POINTS;
        case PrimitiveType::LINES:
            return GL_LINES;
        case PrimitiveType::LINE_LOOP:
            return GL_LINE_LOOP;
        case PrimitiveType::LINE_STRIP:
            return GL_LINE_STRIP;
        case PrimitiveType::TRIANGLES:
            return GL_TRIANGLES;
        case PrimitiveType::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case PrimitiveType::LINES_ADJACENCY:
            return GL_LINES_ADJACENCY;
        case PrimitiveType::LINE_STRIP_ADJACENCY:
            return GL_LINE_STRIP_ADJACENCY;
        case PrimitiveType::TRIANGLES_ADJACENCY:
            return GL_TRIANGLES_ADJACENCY;
        case PrimitiveType::TRIANGLE_STRIP_ADJACENCY:
            return GL_TRIANGLE_STRIP_ADJACENCY;
        default:
            return GL_TRIANGLES;
    }
}

GLenum givr::getBufferUsageType(BufferUsageType const &d) {
    switch(d) {
        case BufferUsageType::STATIC_DRAW:
            return GL_STATIC_DRAW;
        case BufferUsageType::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        default:
            return GL_STATIC_DRAW;
    }

}
