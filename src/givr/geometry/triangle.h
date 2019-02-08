#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    struct Triangle {
        vec3f p1;
        vec3f p2;
        vec3f p3;

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;
        };
    };

    Triangle::Data generateGeometry(Triangle const &t);
};// end namespace givr
