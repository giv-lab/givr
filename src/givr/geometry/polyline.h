#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace givr {

    struct polyline {
        std::vector<float> vertices;
        struct data {
            std::vector<float> vertices;
        };

    };


    polyline::data generate_geometry(polyline const &s);

};// end namespace givr
