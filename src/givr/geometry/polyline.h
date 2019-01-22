#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    template <primitive_type LineType>
    struct polyline {
        std::vector<vec3f> vertices;

        static_assert(
            LineType != primitive_type::LINE_LOOP ||
            LineType != primitive_type::LINE_STRIP ||
            LineType != primitive_type::LINE_STRIP_ADJACENCY,
            "polyline only supports LINE_LOOP, LINE_STRIP or LINE_STRIP_ADJACENCY"
        );

        struct data : public vertex_array_data<LineType> {
            buffer_usage_type vertices_type;
            std::vector<float> vertices;
        };
    };

    template <primitive_type LineType>
    typename polyline<LineType>::data generate_geometry(polyline<LineType> const &l) {
        typename polyline<LineType>::data data;
        data.vertices.reserve(l.vertices.size()*3);
        for (std::size_t i = 0; i < l.vertices.size(); ++i) {
            data.vertices.push_back(l.vertices[i][0]);
            data.vertices.push_back(l.vertices[i][1]);
            data.vertices.push_back(l.vertices[i][2]);
        }
        return data;
    }
};// end namespace givr
