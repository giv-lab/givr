#include "multiline.h"

using MultiLineGeometry = givr::geometry::MultiLineGeometry;
using LineGeometry = givr::geometry::LineGeometry;

void MultiLineGeometry::push_back(LineGeometry l) {
    m_segments.push_back(l);
}

MultiLineGeometry::Data givr::geometry::generateGeometry(MultiLineGeometry const &l) {
    MultiLineGeometry::Data data;
    auto const &segments = l.segments();
    data.vertices.reserve(6 * segments.size());
    auto push_vertex = [&](vec3f const &p) {
        for(std::size_t i = 0; i < 3; ++i) {
            data.vertices.push_back(p[i]);
        }
    };
    for(const auto &segment : segments) {
        push_vertex(segment.p1());
        push_vertex(segment.p2());
    }
    return data;
}
