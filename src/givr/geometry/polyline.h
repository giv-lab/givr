#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    template <PrimitiveType LineType>
    struct PolyLine {
        std::vector<vec3f> vertices;

        static_assert(
            LineType != PrimitiveType::LINE_LOOP ||
            LineType != PrimitiveType::LINE_STRIP,
            "PolyLine only supports LINE_LOOP or LINE_STRIP"
        );

        struct Data : public VertextArrayData<LineType> {
            BufferUsageType verticesType;
            std::vector<float> vertices;
        };
    };

    template <PrimitiveType LineType>
    typename PolyLine<LineType>::Data generateGeometry(PolyLine<LineType> const &l) {
        typename PolyLine<LineType>::Data data;
        data.vertices.reserve(l.vertices.size()*3);
        for (std::size_t i = 0; i < l.vertices.size(); ++i) {
            data.vertices.push_back(l.vertices[i][0]);
            data.vertices.push_back(l.vertices[i][1]);
            data.vertices.push_back(l.vertices[i][2]);
        }
        return data;
    }
};// end namespace givr
