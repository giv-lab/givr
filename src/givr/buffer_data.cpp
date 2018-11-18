#include "buffer_data.h"

using buffer_data = givr::buffer_data;

void buffer_data::add_indices(std::vector<GLushort> const &new_indices) {
    indices.insert(indices.end(), new_indices.begin(), new_indices.end());
}
void buffer_data::add_vertices(std::vector<float> const &new_vertices) {
    vertices.insert(vertices.end(), new_vertices.begin(), new_vertices.end());
}
void buffer_data::add_vertices(std::vector<vec3f> const &new_vertices) {
    vertices.resize(new_vertices.size()*3);
    for (size_t i = 0; i < new_vertices.size(); ++i) {
        vertices[(i*3)] = new_vertices[i][0];
        vertices[(i*3)+1] = new_vertices[i][1];
        vertices[(i*3)+2] = new_vertices[i][2];
    }
}
void buffer_data::add_normals(std::vector<float> const &new_normals) {
    normals.insert(normals.end(), new_normals.begin(), new_normals.end());
}
void buffer_data::add_normals(std::vector<vec3f> const &new_normals) {
    normals.resize(new_normals.size()*3);
    for (size_t i = 0; i < new_normals.size(); ++i) {
        normals[(i*3)] = new_normals[i][0];
        normals[(i*3)+1] = new_normals[i][1];
        normals[(i*3)+2] = new_normals[i][2];
    }
}
void buffer_data::add_uvs(std::vector<float> const &new_uvs) {
    uvs.insert(uvs.end(), new_uvs.begin(), new_uvs.end());
}
/*TODO: void buffer_data::add_uvs(std::vector<vec3f> const &new_uvs) {
    uvs.reserve(new_uvs.size()*2);
    for (size_t i = 0; i < uvs.size(); ++i) {
        uvs[(i*3)] = new_uvs[i][0];
        uvs[(i*3)+1] = new_uvs[i][1];
    }
}*/
void buffer_data::add_colours(std::vector<float> const &new_colours) {
    colours.insert(colours.end(), new_colours.begin(), new_colours.end());
}
void buffer_data::add_colours(std::vector<vec3f> const &new_colours) {
    colours.resize(new_colours.size()*3);
    for (size_t i = 0; i < new_colours.size(); ++i) {
        colours[(i*3)] = new_colours[i][0];
        colours[(i*3)+1] = new_colours[i][1];
        colours[(i*3)+2] = new_colours[i][2];
    }
}
