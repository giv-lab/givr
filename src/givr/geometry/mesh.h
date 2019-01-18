#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include "../types.h"

namespace givr {
    struct mesh {
        std::string filename;

        struct data {
            buffer_usage_type vertices_type;
            buffer_usage_type normals_type;
            buffer_usage_type indices_type;
            buffer_usage_type colours_type;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> colours;
            std::vector<float> uvs;
        };
    };

    mesh::data generate_geometry(const mesh& m);

};// end namespace givr
