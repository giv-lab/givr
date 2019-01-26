#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    struct cylinder {
        vec3f p1;
        vec3f p2;
        float radius = 1.0f;
        std::size_t points = 20;

        struct data : public vertex_array_data<primitive_type::TRIANGLES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> vertices;

            buffer_usage_type normals_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> normals;

            buffer_usage_type indices_type = buffer_usage_type::STATIC_DRAW;
            std::vector<std::uint32_t> indices;
        };
    };

    cylinder::data generate_geometry(cylinder const &l);
};// end namespace givr
