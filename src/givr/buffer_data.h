#pragma once

#include <vector>

#include <glad/glad.h>
#include "types.h"

namespace givr {
    struct buffer_data {
        // Holds the necessary vertex buffer data
        // if the size is zero, then it is not used and will not be supplied to the shader

        buffer_usage_type indices_type;
        std::vector<GLushort> indices;
        buffer_usage_type vertices_type;
        std::vector<float> vertices;
        buffer_usage_type normals_type;
        std::vector<float> normals;
        buffer_usage_type uvs_type;
        std::vector<float> uvs;
        buffer_usage_type colours_type;
        std::vector<float> colours;

        void add_indices(std::vector<GLushort> const &new_indices);
        void add_vertices(std::vector<float> const &new_vertices);
        void add_normals(std::vector<float> const &new_normals);
        void add_uvs(std::vector<float> const &new_uvs);
        void add_colours(std::vector<float> const &new_colours);
    }; // end struct buffer_data
};// end namespace givr
