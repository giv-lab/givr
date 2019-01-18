#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

#include "../types.h"

namespace givr {

    struct custom_geometry {
        std::vector<vec3f> vertices;
        std::vector<vec3f> normals;
        std::vector<std::uint32_t> indices;
        std::vector<vec3f> colours;
        std::vector<vec2f> uvs;

        // TODO: add simpler ways to construct this.

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

    custom_geometry::data generate_geometry(custom_geometry const &s);

};// end namespace givr
