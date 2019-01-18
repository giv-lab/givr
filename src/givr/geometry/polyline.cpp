#include "polyline.h"
#include "tiny_obj_loader.h"

#include <cassert>
#include <cmath>
#include <sstream>
#include <string>

using polyline = givr::polyline;

polyline::data givr::generate_geometry(polyline const &l) {
    polyline::data data;
    data.vertices.reserve(l.vertices.size()*3);
    for (std::size_t i = 0; i < l.vertices.size(); ++i) {
        data.vertices.push_back(l.vertices[i][0]);
        data.vertices.push_back(l.vertices[i][1]);
        data.vertices.push_back(l.vertices[i][2]);
    }
    return data;
}

