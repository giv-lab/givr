#pragma once

#include <vector>
#include <utility>
#include <gsl/span>

#include "gl.h"
#include "types.h"

namespace givr {
    struct BufferData {
        // Holds the necessary vertex buffer data
        // if the size is zero, then it is not used and will not be supplied to the shader
        std::uint16_t dimensions = 3;

        BufferUsageType indicesType;
        gsl::span<const std::uint32_t> indices;
        BufferUsageType verticesType;
        gsl::span<const float> vertices;
        BufferUsageType normalsType;
        gsl::span<const float> normals;
        BufferUsageType uvsType;
        gsl::span<const float> uvs;
        BufferUsageType coloursType;
        gsl::span<const float> colours;


        void addIndices(gsl::span<const std::uint32_t> const &newIndices);
        void addVertices(gsl::span<const float> const &newVertices);
        void addVertices(std::vector<vec3f> const &newVertices);
        void addNormals(gsl::span<const float> const &newNormals);
        void addNormals(std::vector<vec3f> const &newNormals);
        void addUvs(gsl::span<const float> const &newUvs);
        void addUvs(std::vector<vec2f> const &newUvs);
        void addColours(gsl::span<const float> const &newColours);
        void addColours(std::vector<vec3f> const &newColours);
    }; // end struct BufferData
};// end namespace givr
