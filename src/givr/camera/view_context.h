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


template <typename CameraT>
mat4f getViewMatrix(CameraT const &c) {
    return CameraT::getViewMatrix(c);
}

template <typename CameraT>
vec3f getViewPosition(CameraT const &c) {
    return CameraT::getViewPosition(c);
}

template <typename ProjectionT>
vec3f getProjectionMatrix(ProjectionT const &p) {
    return ProjectionT::getProjectionMatrix(p);
}

}// end namespace givr
