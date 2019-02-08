#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    struct Line {
        vec3f p1;
        vec3f p2;

        struct Data : public VertextArrayData<PrimitiveType::LINES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;
        };
    };

    Line::Data generateGeometry(Line const &l);
};// end namespace givr
