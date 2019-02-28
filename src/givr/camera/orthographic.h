#pragma once

#include "../types.h"
#include "parameters.h"
#include "projection.h"

namespace givr {
namespace camera {

    struct OrthographicProjection
        : public Projection<
              Left, Right, Bottom, Top, NearDistance, FarDistance, AspectRatio>
    {
        OrthographicProjection() {
            set(Left(-1.0));
            set(Right(1.0));
            set(Bottom(-1.0));
            set(Top(1.0));
            set(NearDistance(0.1f));
            set(FarDistance(1000.f));
            set(AspectRatio(1.0f));
        }

        float const &left() const { return value<Left>().value(); }
        float const &right() const { return value<Right>().value(); }
        float const &top() const { return value<Top>().value(); }
        float const &bottom() const { return value<Bottom>().value(); }
        float const &aspectRatio() const { return value<AspectRatio>().value(); }
        float const &nearDistance() const { return value<NearDistance>().value(); }
        float const &farDistance() const { return value<FarDistance>().value(); }

        float &left() { return value<Left>().value(); }
        float &right() { return value<Right>().value(); }
        float &top() { return value<Top>().value(); }
        float &bottom() { return value<Bottom>().value(); }
        float &aspectRatio() { return value<AspectRatio>().value(); }
        float &nearDistance() { return value<NearDistance>().value(); }
        float &farDistance() { return value<FarDistance>().value(); }

        void updateAspectRatio(int width, int height);
        mat4f projectionMatrix() const;
    };

    template <typename... Args>
    OrthographicProjection Orthographic(Args &&... args) {
        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(
            is_subset_of<std::tuple<Args...>, OrthographicProjection::Args> &&
            sizeof...(args) <= std::tuple_size<OrthographicProjection::Args>::value,
            "You have provided incorrect parameters for Orthographic. "
            "Left, AspectRatio, NearDistance, FarDistance are optional.");
        OrthographicProjection pv;

        if constexpr (sizeof...(args) > 0) {
            pv.set(std::forward<Args>(args)...);
        }
        return pv;
    }



}// end namespace OrthographicProjection
}// end namespace givr
