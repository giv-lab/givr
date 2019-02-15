#pragma once

#include "../types.h"

namespace givr {
namespace camera {
    template <typename CameraT, typename ProjectionT>
    struct ViewContext {
        CameraT camera;
        ProjectionT projection;
    };

    template <typename CameraT, typename ProjectionT>
    ViewContext<CameraT, ProjectionT> View(CameraT const &camera, ProjectionT const &projection) {
        return ViewContext<CameraT, ProjectionT>{camera, projection};
    }
}// end namespace camera
}// end namespace givr
