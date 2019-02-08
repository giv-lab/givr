#pragma once
#include <cmath>

#include "../types.h"

namespace givr {
    constexpr const float LONGITUDE_MAX = 2. * M_PI;
    constexpr const float LATITUDE_MAX = M_PI;

    struct TurnTable {
        // TODO: ensure these are in the right ranges.
        float latitude = LATITUDE_MAX / 2.;
        float longitude = 0.f;//LONGITUDE_MAX / 2.;
        float radius = 75.f;
        vec3f translation = vec3f{ 0.f, 0.f, 0.f };

        void rotateAroundXPercent(float perc);
        void rotateAroundYPercent(float perc);
        void rotateAroundX(float angle);
        void rotateAroundY(float angle);
        void zoom(float amount);
        void translate(vec3f amount);
    };

    mat4f getViewMatrix(TurnTable const & t);

    vec3f getViewPosition(TurnTable const & t);

}; // end namespace givr
