#include "triangle_soup.h"

using triangle_soup = givr::triangle_soup;

triangle_soup::data givr::generate_geometry(triangle_soup const &t) {
    typename triangle_soup::data data;
    data.vertices.reserve(9 * t.triangles.size());
    auto push_vertex = [&](std::vector<float> &v, vec3f const &p) {
        for(std::size_t i = 0; i < 3; ++i) {
            v.push_back(p[i]);
        }
    };
    for(const auto &tri : t.triangles) {
        auto normal = glm::normalize(glm::cross(tri.p3-tri.p1, tri.p2-tri.p1));

        push_vertex(data.vertices, tri.p1);
        push_vertex(data.normals, normal);
        push_vertex(data.vertices, tri.p2);
        push_vertex(data.normals, normal);
        push_vertex(data.vertices, tri.p3);
        push_vertex(data.normals, normal);
    }
    return data;
}
