#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include "../types.h"
#include "../vertex_array_data.h"

namespace givr {
    struct Mesh {
        std::string filename;

        struct Data : VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> uvs;
        };
    };

    Mesh::Data generateGeometry(const Mesh& m);

};// end namespace givr
