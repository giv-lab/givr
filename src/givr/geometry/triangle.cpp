#include "triangle.h"

using triangle = givr::triangle;

triangle::data givr::generate_geometry(triangle const &t) {
    triangle::data data;
    vec3f normal = glm::normalize(glm::cross(t.p2-t.p1, t.p3-t.p1));
    data.normals.reserve(9);
    data.vertices.reserve(9);
    auto push_vertex = [&](vec3f const &p) {
        for(std::size_t i = 0; i < 3; ++i) {
            data.vertices.push_back(p[i]);
            data.normals.push_back(normal[i]);
        }
    };
    push_vertex(t.p1);
    push_vertex(t.p2);
    push_vertex(t.p3);
    return data;
}
