#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../types.h"

namespace givr {

    struct sphere {
        std::size_t azimuthPoints = 40;
        std::size_t altitudePoints = 20;

        struct data {
            buffer_usage_type vertices_type;
            buffer_usage_type normals_type;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
        };

    };


    sphere::data generate_geometry(sphere const &s);

};// end namespace givr
