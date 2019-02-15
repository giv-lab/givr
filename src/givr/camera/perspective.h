#pragma once

#include "../types.h"
#include "parameters.h"
#include "projection.h"

namespace givr {
namespace camera {

    struct PerspectiveProjection
        : public Projection<FieldOfViewY, AspectRatio, NearDistance, FarDistance>
    {
        PerspectiveProjection() {
            set(FieldOfViewY(45.f));
            set(AspectRatio(4.f/3.f));
            set(NearDistance(0.1f));
            set(FarDistance(1000.f));
        }

        float const &fieldOfViewY() const { return value<FieldOfViewY>().value(); }
        float const &aspectRatio() const { return value<AspectRatio>().value(); }
        float const &nearDistance() const { return value<NearDistance>().value(); }
        float const &farDistance() const { return value<FarDistance>().value(); }

        float &fieldOfViewY() { return value<FieldOfViewY>().value(); }
        float &aspectRatio() { return value<AspectRatio>().value(); }
        float &nearDistance() { return value<NearDistance>().value(); }
        float &farDistance() { return value<FarDistance>().value(); }
        void updateAspectRatio(int width, int height);

        mat4f projectionMatrix() const;
    };

    template <typename... Args>
    PerspectiveProjection Perspective(Args &&... args) {
        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(
            is_subset_of<std::tuple<Args...>, PerspectiveProjection::Args> &&
            sizeof...(args) <= std::tuple_size<PerspectiveProjection::Args>::value,
            "You have provided incorrect parameters for Perspective. "
            "FieldOfViewY, AspectRatio, NearDistance, FarDistance are optional.");
        PerspectiveProjection pv;

        if constexpr (sizeof...(args) > 0) {
            pv.set(std::forward<Args>(args)...);
        }
        return pv;
    }



}// end namespace camera
}// end namespace givr
