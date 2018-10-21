#pragma once

namespace givr {
    template <typename CameraT, typename ProjectionT>
    struct view_context {
        CameraT camera;
        ProjectionT projection;
    };
};// end namespace givr
