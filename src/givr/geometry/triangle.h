#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    struct triangle {
        vec3f p1;
        vec3f p2;
        vec3f p3;

        struct data : public vertex_array_data<primitive_type::TRIANGLES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> vertices;

            buffer_usage_type normals_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> normals;
        };
    };

    triangle::data generate_geometry(triangle const &t);
};// end namespace givr
