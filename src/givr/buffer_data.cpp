#include "buffer_data.h"

using BufferData = givr::BufferData;
using vec3f = givr::vec3f;

void BufferData::addIndices(std::vector<GLuint> const &newIndices) {
    indices.insert(indices.end(), newIndices.begin(), newIndices.end());
}
void BufferData::addVertices(std::vector<float> const &newVertices) {
    vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
}
void BufferData::addVertices(std::vector<vec3f> const &newVertices) {
    vertices.resize(newVertices.size()*3);
    for (size_t i = 0; i < newVertices.size(); ++i) {
        vertices[(i*3)] = newVertices[i][0];
        vertices[(i*3)+1] = newVertices[i][1];
        vertices[(i*3)+2] = newVertices[i][2];
    }
}
void BufferData::addNormals(std::vector<float> const &newNormals) {
    normals.insert(normals.end(), newNormals.begin(), newNormals.end());
}
void BufferData::addNormals(std::vector<vec3f> const &newNormals) {
    normals.resize(newNormals.size()*3);
    for (size_t i = 0; i < newNormals.size(); ++i) {
        normals[(i*3)] = newNormals[i][0];
        normals[(i*3)+1] = newNormals[i][1];
        normals[(i*3)+2] = newNormals[i][2];
    }
}
void BufferData::addUvs(std::vector<float> const &newUvs) {
    uvs.insert(uvs.end(), newUvs.begin(), newUvs.end());
}
/*TODO: void BufferData::addUvs(std::vector<vec3f> const &newUvs) {
    uvs.reserve(newUvs.size()*2);
    for (size_t i = 0; i < uvs.size(); ++i) {
        uvs[(i*3)] = newUvs[i][0];
        uvs[(i*3)+1] = newUvs[i][1];
    }
}*/
void BufferData::addColours(std::vector<float> const &newColours) {
    colours.insert(colours.end(), newColours.begin(), newColours.end());
}
void BufferData::addColours(std::vector<vec3f> const &newColours) {
    colours.resize(newColours.size()*3);
    for (size_t i = 0; i < newColours.size(); ++i) {
        colours[(i*3)] = newColours[i][0];
        colours[(i*3)+1] = newColours[i][1];
        colours[(i*3)+2] = newColours[i][2];
    }
}
