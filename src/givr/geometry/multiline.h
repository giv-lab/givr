#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "line.h"

namespace givr {

    struct multiline {
        std::vector<line> segments;

        void add_line(line l);
        void add_line(vec3f const &p1, vec3f const &p2);


        struct data : public vertex_array_data<primitive_type::LINES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            std::vector<float> vertices;
        };
    };

    multiline::data generate_geometry(multiline const &l);
};// end namespace givr
