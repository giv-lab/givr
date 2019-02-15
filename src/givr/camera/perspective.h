#pragma once

#include "../types.h"
#include "parameters.h"
#include "projection.h"

namespace givr {
namespace camera {

    struct PerspectiveViewProjection
        : public Projection<FieldOfViewY, AspectRatio, NearDistance, FarDistance>
    {
        float const &fieldOfViewY() const { return value<FieldOfViewY>().value(); }
        float const &aspectRatio() const { return value<AspectRatio>().value(); }
        float const &nearDistance() const { return value<NearDistance>().value(); }
        float const &farDistance() const { return value<FarDistance>().value(); }

        float &fieldOfViewY() { return value<FieldOfViewY>().value(); }
        float &aspectRatio() { return value<AspectRatio>().value(); }
        float &nearDistance() { return value<NearDistance>().value(); }
        float &farDistance() { return value<FarDistance>().value(); }
        void updateAspectRatio(int width, int height);
    };

    template <typename... Args>
    PerspectiveViewProjection PerspectiveView(Args &&... args) {
        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(
            is_subset_of<std::tuple<Args...>, PerspectiveViewProjection::Args> &&
            sizeof...(args) <= std::tuple_size<PerspectiveViewProjection::Args>::value,
            "You have provided incorrect parameters for PerspectiveView. "
            "FieldOfViewY, AspectRatio, NearDistance, FarDistance are optional.");
        PerspectiveViewProjection pv;

        pv.set(FieldOfViewY(45.f));
        pv.set(AspectRatio(4.f/3.f));
        pv.set(NearDistance(0.1f));
        pv.set(FarDistance(1000.f));
        if constexpr (sizeof...(args) > 0) {
            pv.set(std::forward<Args>(args)...);
        }
        return pv;
    }


    mat4f getProjectionMatrix(PerspectiveViewProjection const &t);

}// end namespace camera
}// end namespace givr
