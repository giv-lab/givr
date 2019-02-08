#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    struct Cylinder {
        vec3f p1;
        vec3f p2;
        float radius = 1.0f;
        std::size_t points = 20;

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;

            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;
            std::vector<std::uint32_t> indices;
        };
    };

    Cylinder::Data generateGeometry(Cylinder const &l);
};// end namespace givr
