#include "buffer_data.h"

using buffer_data = givr::buffer_data;

void buffer_data::add_indices(std::vector<GLushort> const &new_indices) {
    indices.insert(indices.end(), new_indices.begin(), new_indices.end());
}
void buffer_data::add_vertices(std::vector<float> const &new_vertices) {
    vertices.insert(vertices.end(), new_vertices.begin(), new_vertices.end());
}
void buffer_data::add_normals(std::vector<float> const &new_normals) {
    normals.insert(normals.end(), new_normals.begin(), new_normals.end());
}
void buffer_data::add_uvs(std::vector<float> const &new_uvs) {
    uvs.insert(uvs.end(), new_uvs.begin(), new_uvs.end());
}
void buffer_data::add_colours(std::vector<float> const &new_colours) {
    colours.insert(colours.end(), new_colours.begin(), new_colours.end());
}
