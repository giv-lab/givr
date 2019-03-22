#include "buffer_data.h"

using BufferData = givr::BufferData;
using vec3f = givr::vec3f;

void BufferData::addIndices(gsl::span<const std::uint32_t> const &newIndices) {
    indices = newIndices;
}

void BufferData::addVertices(gsl::span<const float> const &newVertices) {
    vertices = newVertices;
}

void BufferData::addVertices(std::vector<vec3f> const &newVertices) {
    vertices = gsl::span<const float>(
        reinterpret_cast<float const *>(newVertices.data()),
        newVertices.size()*3
    );
}

void BufferData::addNormals(gsl::span<const float> const &newNormals) {
    normals = newNormals;
}

void BufferData::addNormals(std::vector<vec3f> const &newNormals) {
    normals = gsl::span<const float>(
        reinterpret_cast<float const *>(newNormals.data()),
        newNormals.size()*3
    );
}

void BufferData::addUvs(gsl::span<const float> const &newUvs) {
    uvs = newUvs;
}

void BufferData::addUvs(std::vector<vec2f> const &newUvs) {
    uvs = gsl::span<const float>(
        reinterpret_cast<float const *>(newUvs.data()),
        newUvs.size()*2
    );
}
void BufferData::addColours(gsl::span<const float> const &newColours) {
    colours = newColours;
}
void BufferData::addColours(std::vector<vec3f> const &newColours) {
    colours = gsl::span<const float>(
        reinterpret_cast<float const *>(newColours.data()),
        newColours.size()*3
    );
}


