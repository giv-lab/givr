#pragma once

#include "../types.h"

namespace givr {
    template <typename CameraT, typename ProjectionT>
    struct view_context {
        CameraT camera;
        ProjectionT projection;
    };
    template <typename CameraT>
    mat4f get_view_matrix(CameraT const &c) {
        return CameraT::get_view_matrix(c);
    }
    template <typename CameraT>
    vec3f get_view_position(CameraT const &c) {
        return CameraT::get_view_position(c);
    }
    template <typename ProjectionT>
    vec3f get_projection_matrix(ProjectionT const &p) {
        return ProjectionT::get_projection_matrix(p);
    }
};// end namespace givr
