#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "triangle.h"

namespace givr {

    struct triangle_soup {
        std::vector<triangle> triangles;

        void add_line(triangle t) {
            triangles.push_back(t);
        }
        void add_line(vec3f const &p1, vec3f const &p2, vec3f const &p3) {
            triangles.push_back(triangle{p1, p2, p3});
        }

        struct data : public vertex_array_data<primitive_type::TRIANGLES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> vertices;

            buffer_usage_type normals_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> normals;
        };
    };

    triangle_soup::data generate_geometry(triangle_soup const &t);
};// end namespace givr
