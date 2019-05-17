#pragma once

#include "types.h"

// This class is used for compile time checking that the data
// is compatible with the style.
namespace givr {
template <givr::PrimitiveType PrimitiveValue>
class VertexArrayData {
};

// A constexpr function for determining the primitive type from
// the geometry type.
template <typename GeometryT>
constexpr givr::PrimitiveType getPrimitive() {
    typedef givr::PrimitiveType pt;
    typedef typename GeometryT::Data Geometry;
    if constexpr (std::is_base_of<VertexArrayData<pt::POINTS>, Geometry>::value) {
        return PrimitiveType::POINTS;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINES>, Geometry>::value) {
        return PrimitiveType::LINES;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINE_LOOP>, Geometry>::value) {
        return PrimitiveType::LINE_LOOP;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINE_STRIP>, Geometry>::value) {
        return PrimitiveType::LINE_STRIP;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLES>, Geometry>::value) {
        return PrimitiveType::TRIANGLES;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLE_STRIP>, Geometry>::value) {
        return PrimitiveType::TRIANGLE_STRIP;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLE_FAN>, Geometry>::value) {
        return PrimitiveType::TRIANGLE_FAN;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINES_ADJACENCY>, Geometry>::value) {
        return PrimitiveType::LINES_ADJACENCY;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINE_STRIP_ADJACENCY>, Geometry>::value) {
        return PrimitiveType::LINE_STRIP_ADJACENCY;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLES_ADJACENCY>, Geometry>::value) {
        return PrimitiveType::TRIANGLES_ADJACENCY;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLE_STRIP_ADJACENCY>, Geometry>::value) {
        return PrimitiveType::TRIANGLE_STRIP_ADJACENCY;
    } else {
        // We can guarantee we can render this.
        // TODO: add a compile time assert here
        return PrimitiveType::POINTS;
    }
}

// A constexpr function which determines if the geometry type is line based
template <typename GeometryT>
constexpr bool isLineBased() {
    typedef givr::PrimitiveType pt;
    typedef typename GeometryT::Data Geometry;
    if constexpr (std::is_base_of<VertexArrayData<pt::LINES>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINE_LOOP>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINE_STRIP>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINES_ADJACENCY>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::LINE_STRIP_ADJACENCY>, Geometry>::value) {
        return true;
    } else {
        return false;
    }
}


// A constexpr function which determines if the geometry type is triangle based
template <typename GeometryT>
constexpr bool isTriangleBased() {
    typedef givr::PrimitiveType pt;
    typedef typename GeometryT::Data Geometry;
    if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLES>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLE_STRIP>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLE_FAN>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLES_ADJACENCY>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<VertexArrayData<pt::TRIANGLE_STRIP_ADJACENCY>, Geometry>::value) {
        return true;
    } else {
        return false;
    }
}

// A constexpr function which determines if the geometry type is point based
template <typename GeometryT>
constexpr bool isPointBased() {
    typedef givr::PrimitiveType pt;
    typedef typename GeometryT::Data Geometry;
    if constexpr (std::is_base_of<VertexArrayData<pt::POINTS>, Geometry>::value) {
        return true;
    } else {
        return false;
    }
}
};
