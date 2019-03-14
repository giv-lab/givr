#include "quad.h"

using QuadGeometry = givr::geometry::QuadGeometry;

QuadGeometry::Data givr::geometry::generateGeometry(QuadGeometry const &t) {
    QuadGeometry::Data data;
    vec3f normal1 = glm::normalize(glm::cross(t.p3()-t.p2(), t.p1()-t.p2()));
    vec3f normal2 = glm::normalize(glm::cross(t.p4()-t.p3(), t.p1()-t.p3()));
    vec3f avg = glm::normalize(normal1 + normal2);
    data.uvs.reserve(8);
    data.normals.reserve(12);
    data.vertices.reserve(12);
    auto push_vertex = [&](vec3f const &p, vec3f const &n) {
        for(std::size_t i = 0; i < 3; ++i) {
            data.vertices.push_back(p[i]);
            data.normals.push_back(n[i]);
        }
    };
    auto push_uvs = [&](float u, float v) {
        data.uvs.push_back(u);
        data.uvs.push_back(v);
    };
    auto add_tri = [&](std::uint32_t i, std::uint32_t j, std::uint32_t k) {
        data.indices.push_back(i);
        data.indices.push_back(j);
        data.indices.push_back(k);
    };
    push_vertex(t.p1(), normal1);
    push_uvs(0.0, 0.0);
    push_vertex(t.p2(), avg);
    push_uvs(0.0, 1.0);
    push_vertex(t.p3(), avg);
    push_uvs(1.0, 1.0);
    push_vertex(t.p4(), normal2);
    push_uvs(1.0, 0.0);

    add_tri(0, 1, 2);
    add_tri(0, 2, 3);
    return data;
}
