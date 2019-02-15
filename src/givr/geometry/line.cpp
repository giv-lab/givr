#include "line.h"

using LineGeometry = givr::geometry::LineGeometry;
LineGeometry::Data givr::geometry::generateGeometry(LineGeometry const &l) {
    LineGeometry::Data data;
    data.vertices.reserve(6);
    auto push_vertex = [&](vec3f const &p) {
        for(std::size_t i = 0; i < 3; ++i) {
            data.vertices.push_back(p[i]);
        }
    };
    push_vertex(l.p1());
    push_vertex(l.p2());
    return data;
}
