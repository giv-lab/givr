#include "triangle_soup.h"

using TriangleSoupGeometry = givr::geometry::TriangleSoupGeometry;
using namespace givr::geometry;

TriangleSoupGeometry::Data givr::geometry::generateGeometry(TriangleSoupGeometry const &t) {
    typename TriangleSoupGeometry::Data data;
    data.vertices.reserve(9 * t.triangles().size());
    auto push_vertex = [&](std::vector<float> &v, vec3f const &p) {
        for(std::size_t i = 0; i < 3; ++i) {
            v.push_back(p[i]);
        }
    };
    for(const auto &tri : t.triangles()) {
        auto normal = glm::normalize(glm::cross(tri.p2()-tri.p1(), tri.p3()-tri.p1()));

        push_vertex(data.vertices, tri.value<Point1>());
        push_vertex(data.normals, normal);
        push_vertex(data.vertices, tri.value<Point2>());
        push_vertex(data.normals, normal);
        push_vertex(data.vertices, tri.value<Point3>());
        push_vertex(data.normals, normal);
    }
    return data;
}
