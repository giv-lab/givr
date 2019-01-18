#include "custom.h"
#include "tiny_obj_loader.h"

#include <cassert>
#include <cmath>
#include <sstream>
#include <string>

using custom_geometry = givr::custom_geometry;

template<typename VectorT, int vecsize>
void custom_geometry_copy(std::vector<VectorT> const &source, std::vector<float> &target) {
    for (std::size_t i = 0; i < source.size(); ++i) {
        for (std::size_t j = 0; j < vecsize; ++j) {
            target.push_back(source[i][j]);
        }
    }
};

custom_geometry::data givr::generate_geometry(custom_geometry const &l) {
    custom_geometry::data data;

    custom_geometry_copy<vec3f, 3>(l.vertices, data.vertices);
    custom_geometry_copy<vec3f, 3>(l.normals, data.normals);
    custom_geometry_copy<vec3f, 3>(l.colours, data.colours);
    custom_geometry_copy<vec2f, 2>(l.uvs, data.uvs);

    data.indices.insert(data.indices.end(), l.indices.begin(), l.indices.end());

    return data;
}

