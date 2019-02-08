#pragma once

#include "../types.h"

namespace givr {

    struct PerspectiveView {
        float fovy = 45.f;
        float aspect = 4.f / 3.f;
        float nearDistance = 0.1f;
        float farDistance = 1000.f;
        void updateAspectRatio(int width, int height);
    };

    mat4f getProjectionMatrix(PerspectiveView const &t);

};// end namespace givr
