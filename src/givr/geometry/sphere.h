#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../types.h"
#include "../vertex_array_data.h"

namespace givr {

    struct Sphere {
        std::size_t azimuthPoints = 20;
        std::size_t altitudePoints = 20;

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType uvsType = BufferUsageType::STATIC_DRAW;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> uvs;
        };

    };


    Sphere::Data generateGeometry(Sphere const &s);

};// end namespace givr
