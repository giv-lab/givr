#pragma once

#include "../types.h"
#include "som.h"

namespace givr {

    struct perspective_view {
        float fovy = 45.f;
        float aspect = 4.f / 3.f;
        float near_distance = 0.1f;
        float far_distance = 1000.f;
        void update_aspect_ratio(int width, int height);
    };

    mat4f get_projection_matrix(perspective_view const &t);

};// end namespace givr
