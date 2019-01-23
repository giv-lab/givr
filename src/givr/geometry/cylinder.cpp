#include "cylinder.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>

using cylinder = givr::cylinder;
cylinder::data givr::generate_geometry(cylinder const &l) {
    cylinder::data data;

    auto add = [&](std::vector<float> &v, vec3f const &p) {
        v.push_back(p[0]);
        v.push_back(p[1]);
        v.push_back(p[2]);
    };

    // Calculate the axis around which we will rotate and a
    // vector which is perpindicular to that axis.
    vec3f axis = l.p2 - l.p1;
    // Find the x,y,z axis along which the axis vector is the shortest
    std::vector<std::size_t> indices{0, 1, 2};
    std::size_t min_i = *std::min_element(
        indices.begin(),
        indices.end(),
        [&](std::size_t const &i_1, std::size_t const &i_2) {
            return std::abs(axis[i_1]) < std::abs(axis[i_2]);
        }
    );
    // Use that axis to define a pseudo-orthogonal vector.
    vec3f radius_vec = vec3f{0.0, 0.0, 0.0};
    radius_vec[min_i] = 1.0f;
    // Use cross product, normalize and then scale by radius to get our vector
    radius_vec = l.radius * glm::normalize(glm::cross(axis, radius_vec));

    // Next, rotate the vector around that axis by l.points times.
    // Start by inserting the first two points
    data.vertices.reserve(l.points*6);
    data.normals.reserve(l.points*6);
    float step = 360.0f/static_cast<float>(l.points);
    for (float x = 0.f; x < 360.0f; x+= step) {
        x = x*0.01745329252f; // convert to radians
        vec3f normal = glm::rotate(radius_vec, x, axis);
        add(data.vertices, l.p2 + normal);
        add(data.vertices, l.p1 + normal);
        add(data.normals, normal);
        add(data.normals, normal);
    }
    // Create the triangles 2 at a time.
    for (std::size_t i = 2; i < data.vertices.size()/3; i+=2) {
        data.indices.push_back(i);
        data.indices.push_back(i-2);
        data.indices.push_back(i-1);

        data.indices.push_back(i+1);
        data.indices.push_back(i);
        data.indices.push_back(i-1);
    }

    // Create the last set
    auto size = data.vertices.size()/3;
    data.indices.push_back(0);
    data.indices.push_back(size-2);
    data.indices.push_back(size-1);

    data.indices.push_back(1);
    data.indices.push_back(0);
    data.indices.push_back(size-1);

    return data;
}
