#pragma once

#include "types.h"

// This class is used for compile time checking that the data
// is compatible with the style.
namespace givr {
template <givr::primitive_type PrimitiveValue>
class vertex_array_data {
};

// A constexpr function for determining the primitive type from
// the geometry type.
template <typename GeometryT>
constexpr givr::primitive_type get_primitive() {
    typedef givr::primitive_type pt;
    typedef typename GeometryT::data Geometry;
    if constexpr (std::is_base_of<vertex_array_data<pt::POINTS>, Geometry>::value) {
        return primitive_type::POINTS;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINES>, Geometry>::value) {
        return primitive_type::LINES;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINE_LOOP>, Geometry>::value) {
        return primitive_type::LINE_LOOP;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINE_STRIP>, Geometry>::value) {
        return primitive_type::LINE_STRIP;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLES>, Geometry>::value) {
        return primitive_type::TRIANGLES;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLE_STRIP>, Geometry>::value) {
        return primitive_type::TRIANGLE_STRIP;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLE_FAN>, Geometry>::value) {
        return primitive_type::TRIANGLE_FAN;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINES_ADJACENCY>, Geometry>::value) {
        return primitive_type::LINES_ADJACENCY;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINE_STRIP_ADJACENCY>, Geometry>::value) {
        return primitive_type::LINE_STRIP_ADJACENCY;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLES_ADJACENCY>, Geometry>::value) {
        return primitive_type::TRIANGLES_ADJACENCY;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLE_STRIP_ADJACENCY>, Geometry>::value) {
        return primitive_type::TRIANGLE_STRIP_ADJACENCY;
    } else {
        // We can guarantee we can render this.
        // TODO: add a compile time assert here
        return primitive_type::POINTS;
    }
}

// A constexpr function which determines if the geometry type is line based
template <typename GeometryT>
constexpr bool is_line_based() {
    typedef givr::primitive_type pt;
    typedef typename GeometryT::data Geometry;
    if constexpr (std::is_base_of<vertex_array_data<pt::LINES>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINE_LOOP>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINE_STRIP>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINES_ADJACENCY>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::LINE_STRIP_ADJACENCY>, Geometry>::value) {
        return true;
    } else {
        return false;
    }
}


// A constexpr function which determines if the geometry type is triangle based
template <typename GeometryT>
constexpr bool is_triangle_based() {
    typedef givr::primitive_type pt;
    typedef typename GeometryT::data Geometry;
    if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLES>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLE_STRIP>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLE_FAN>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLES_ADJACENCY>, Geometry>::value) {
        return true;
    } else if constexpr (std::is_base_of<vertex_array_data<pt::TRIANGLE_STRIP_ADJACENCY>, Geometry>::value) {
        return true;
    } else {
        return false;
    }
}

// A constexpr function which determines if the geometry type is point based
template <typename GeometryT>
constexpr bool is_point_based() {
    typedef givr::primitive_type pt;
    typedef typename GeometryT::data Geometry;
    if constexpr (std::is_base_of<vertex_array_data<pt::POINTS>, Geometry>::value) {
        return true;
    } else {
        return false;
    }
}
};
