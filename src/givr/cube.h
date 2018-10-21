#pragma once
#include <cstdint>
#include <vector>

namespace givr {
    struct cube {
    };

    std::vector<float> generate_vertices(cube const &s);
    std::vector<std::uint16_t> generate_indices(cube const &s);

    // TODO: implement the uv and normal functions.
}; // end namespace givr
