#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace givr {

    struct sphere {
        std::size_t azimuthPoints = 40;
        std::size_t altitudePoints = 20;

        struct data {
            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint16_t> indices;
        };

    };


    sphere::data generate_geometry(sphere const &s);

};// end namespace givr
