#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    struct line {
        vec3f p1;
        vec3f p2;

        struct data : public vertex_array_data<primitive_type::LINES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> vertices;
        };
    };

    line::data generate_geometry(line const &l);
};// end namespace givr
