#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "line.h"

namespace givr {

    struct MultiLine {
        std::vector<Line> segments;

        void addLine(Line l);
        void addLine(vec3f const &p1, vec3f const &p2);


        struct Data : public VertextArrayData<PrimitiveType::LINES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;
        };
    };

    MultiLine::Data generateGeometry(MultiLine const &l);
};// end namespace givr
