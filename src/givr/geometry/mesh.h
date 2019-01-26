#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include "../types.h"
#include "../vertex_array_data.h"

namespace givr {
    struct mesh {
        std::string filename;

        struct data : vertex_array_data<primitive_type::TRIANGLES> {
            buffer_usage_type vertices_type = buffer_usage_type::STATIC_DRAW;
            buffer_usage_type normals_type = buffer_usage_type::STATIC_DRAW;
            buffer_usage_type indices_type = buffer_usage_type::STATIC_DRAW;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> uvs;
        };
    };

    mesh::data generate_geometry(const mesh& m);

};// end namespace givr
