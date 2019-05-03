#pragma once

#include <vector>
#include <utility>

#include "gl.h"
#include "types.h"

namespace givr {
    struct BufferData {
        // Holds the necessary vertex buffer data
        // if the size is zero, then it is not used and will not be supplied to the shader
        std::uint16_t dimensions = 3;

        BufferUsageType indicesType;
        std::pair<int, std::uint32_t const *> indices;
        BufferUsageType verticesType;
        std::pair<int, float const *> vertices;
        BufferUsageType normalsType;
        std::pair<int, float const *> normals;
        BufferUsageType uvsType;
        std::pair<int, float const *> uvs;
        BufferUsageType coloursType;
        std::pair<int, float const *> colours;


        void addIndices(std::vector<GLuint> const &newIndices);
        void addIndices(std::pair<int, std::uint32_t const *> const &newIndices);
        void addVertices(std::vector<float> const &newVertices);
        void addVertices(std::vector<vec3f> const &newVertices);
        void addVertices(std::pair<int, float const *> const &newVertices);
        void addNormals(std::vector<float> const &newNormals);
        void addNormals(std::vector<vec3f> const &newNormals);
        void addNormals(std::pair<int, float const *> const &newNormals);
        void addUvs(std::vector<float> const &newUvs);
        void addUvs(std::pair<int, float const *> const &newUvs);
        //TODO: void addUvs(std::vector<vec2f> const &newUvs);
        void addColours(std::vector<float> const &newColours);
        void addColours(std::vector<vec3f> const &newColours);
        void addColours(std::pair<int, float const *> const &newColours);
    }; // end struct BufferData
};// end namespace givr
