#pragma once

#include <vector>

#include "gl.h"
#include "types.h"

namespace givr {
    struct BufferData {
        // Holds the necessary vertex buffer data
        // if the size is zero, then it is not used and will not be supplied to the shader

        BufferUsageType indicesType;
        std::vector<GLuint> indices;
        BufferUsageType verticesType;
        std::vector<float> vertices;
        BufferUsageType normalsType;
        std::vector<float> normals;
        BufferUsageType uvsType;
        std::vector<float> uvs;
        BufferUsageType coloursType;
        std::vector<float> colours;

        void addIndices(std::vector<GLuint> const &new_indices);
        void addVertices(std::vector<float> const &new_vertices);
        void addVertices(std::vector<vec3f> const &new_vertices);
        void addNormals(std::vector<float> const &new_normals);
        void addNormals(std::vector<vec3f> const &new_normals);
        void addUvs(std::vector<float> const &new_uvs);
        //TODO: void addUvs(std::vector<vec2f> const &new_uvs);
        void addColours(std::vector<float> const &new_colours);
        void addColours(std::vector<vec3f> const &new_colours);
    }; // end struct BufferData
};// end namespace givr
