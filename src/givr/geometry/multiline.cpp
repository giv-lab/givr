#include "multiline.h"

using multiline = givr::multiline;

void multiline::add_line(line l) {
    segments.push_back(l);
}
void multiline::add_line(vec3f const &p1, vec3f const &p2) {
    segments.push_back(line{p1, p2});
}

multiline::data givr::generate_geometry(multiline const &l) {
    multiline::data data;
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
