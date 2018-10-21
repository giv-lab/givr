#pragma once

#include <vector>

#include <glad/glad.h>

namespace givr {
    struct buffer_data {
        // Holds the necessary vertex buffer data
        // if the size is zero, then it is not used and will not be supplied to the shader

        std::vector<GLushort> indices;
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<float> colours;

        void add_indices(std::vector<GLushort> const &new_indices);
        void add_vertices(std::vector<float> const &new_vertices);
        void add_normals(std::vector<float> const &new_normals);
        void add_uvs(std::vector<float> const &new_uvs);
        void add_colours(std::vector<float> const &new_colours);
    }; // end struct buffer_data
};// end namespace givr
