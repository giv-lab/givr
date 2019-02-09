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

        void addIndices(std::vector<GLuint> const &newIndices);
        void addVertices(std::vector<float> const &newVertices);
        void addVertices(std::vector<vec3f> const &newVertices);
        void addNormals(std::vector<float> const &newNormals);
        void addNormals(std::vector<vec3f> const &newNormals);
        void addUvs(std::vector<float> const &newUvs);
        //TODO: void addUvs(std::vector<vec2f> const &newUvs);
        void addColours(std::vector<float> const &newColours);
        void addColours(std::vector<vec3f> const &newColours);
    }; // end struct BufferData
};// end namespace givr
