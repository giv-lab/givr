#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

#include "../types.h"

namespace givr {

    struct polyline {
        std::vector<float> vertices;
        struct data {
            buffer_usage_type vertices_type;
            std::vector<float> vertices;
        };

    };


    polyline::data generate_geometry(polyline const &s);

};// end namespace givr
