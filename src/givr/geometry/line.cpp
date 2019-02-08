#include "line.h"

using Line = givr::Line;
Line::Data givr::generateGeometry(Line const &l) {
    Line::Data data;
    data.vertices.reserve(6);
    data.vertices.push_back(l.p1[0]);
    data.vertices.push_back(l.p1[1]);
    data.vertices.push_back(l.p1[2]);
    data.vertices.push_back(l.p2[0]);
    data.vertices.push_back(l.p2[1]);
    data.vertices.push_back(l.p2[2]);
    return data;
}
