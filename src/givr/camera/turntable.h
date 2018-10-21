#pragma once
#include <cmath>

#include "../types.h"
#include "som.h"


namespace givr {
    constexpr const float LONGITUDE_MAX = 2. * M_PI;
    constexpr const float LATITUDE_MAX = M_PI;

    struct turntable {
        // TODO: ensure these are in the right ranges.
        float latitude = LATITUDE_MAX / 2.;
        float longitude = 0.f;//LONGITUDE_MAX / 2.;
        float radius = 75.f;
        vec3f translation = vec3f{0.f, 0.f, 0.f};

        void rotate_around_x_percent(float perc);
        void rotate_around_y_percent(float perc);
        void rotate_around_x(float angle);
        void rotate_around_y(float angle);
        void zoom(float amount);
        void translate(vec3f amount);
    };

    mat4f get_view_matrix(turntable const & t);

}; // end namespace givr
