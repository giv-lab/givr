#include "buffer_data.h"

using BufferData = givr::BufferData;
using vec3f = givr::vec3f;

void BufferData::addIndices(std::vector<GLuint> const &newIndices) {
    indices = std::make_pair(newIndices.size(), newIndices.data());
}

void BufferData::addIndices(std::pair<int, std::uint32_t const *> const &newIndices) {
    indices = newIndices;
}

void BufferData::addVertices(std::vector<float> const &newVertices) {
    vertices = std::make_pair(newVertices.size(), newVertices.data());
}

void BufferData::addVertices(std::vector<vec3f> const &newVertices) {
    vertices = std::make_pair(
        newVertices.size()*3,
        reinterpret_cast<float const *>(newVertices.data())
    );
}

void BufferData::addVertices(std::pair<int, float const *> const &newVertices) {
    vertices = newVertices;
}

void BufferData::addNormals(std::vector<float> const &newNormals) {
    normals = std::make_pair(newNormals.size(), newNormals.data());
}

void BufferData::addNormals(std::vector<vec3f> const &newNormals) {
    normals = std::make_pair(
        newNormals.size()*3,
        reinterpret_cast<float const *>(newNormals.data())
    );
}

void BufferData::addNormals(std::pair<int, float const *> const &newNormals) {
    normals = newNormals;
}

void BufferData::addUvs(std::vector<float> const &newUvs) {
    uvs = std::make_pair(newUvs.size(), newUvs.data());
}

void BufferData::addUvs(std::pair<int, float const *> const &newUvs) {
    uvs = newUvs;
}

/*TODO: void BufferData::addUvs(std::vector<vec3f> const &newUvs) {
    uvs.reserve(newUvs.size()*2);
    for (size_t i = 0; i < uvs.size(); ++i) {
        uvs[(i*3)] = newUvs[i][0];
        uvs[(i*3)+1] = newUvs[i][1];
    }
}*/
void BufferData::addColours(std::vector<float> const &newColours) {
    colours = std::make_pair(newColours.size(), newColours.data());
}
void BufferData::addColours(std::vector<vec3f> const &newColours) {
    colours = std::make_pair(
        newColours.size()*3,
        reinterpret_cast<float const *>(newColours.data())
    );
}

void BufferData::addColours(std::pair<int, float const *> const &newColours) {
    colours = newColours;
}
