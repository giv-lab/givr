#include "multiline.h"

using MultiLine = givr::MultiLine;
using Line = givr::Line;

void MultiLine::addLine(Line l) {
    segments.push_back(l);
}
void MultiLine::addLine(vec3f const &p1, vec3f const &p2) {
    segments.push_back(Line{p1, p2});
}

MultiLine::Data givr::generateGeometry(MultiLine const &l) {
    MultiLine::Data data;
    data.vertices.reserve(6 * l.segments.size());
    auto push_vertex = [&](vec3f const &p) {
        for(std::size_t i = 0; i < 3; ++i) {
            data.vertices.push_back(p[i]);
        }
    };
    for(const auto &segment : l.segments) {
        push_vertex(segment.p1);
        push_vertex(segment.p2);
    }
    return data;
}
