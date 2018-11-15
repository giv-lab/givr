#include "polyline.h"
#include "tiny_obj_loader.h"

#include <cassert>
#include <cmath>
#include <sstream>
#include <string>

using polyline = givr::polyline;

polyline::data givr::generate_geometry(polyline const &l) {
    polyline::data data;
    data.vertices.insert(data.vertices.end(), l.vertices.begin(), l.vertices.end());
    return data;
}

