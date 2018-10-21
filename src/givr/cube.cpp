#include "cube.h"

std::vector<float> givr::generate_vertices(cube const &) {
    return std::vector<float>{
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
        1.0,  1.0, -1.0
    };
}

std::vector<std::uint16_t> givr::generate_indices(cube const &) {
    return std::vector<std::uint16_t>{
        0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
    };
}
