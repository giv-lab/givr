#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../types.h"
#include "../vertex_array_data.h"

namespace givr {

    struct sphere {
        std::size_t azimuthPoints = 20;
        std::size_t altitudePoints = 20;

        struct data : public vertex_array_data<primitive_type::TRIANGLES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            buffer_usage_type normals_type = buffer_usage_type::STATIC_DRAW;
            buffer_usage_type indices_type = buffer_usage_type::STATIC_DRAW;
            buffer_usage_type uvs_type = buffer_usage_type::STATIC_DRAW;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> uvs;
        };

    };


    sphere::data generate_geometry(sphere const &s);

};// end namespace givr
