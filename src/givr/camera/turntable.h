#pragma once
#include <cmath>

#include "../types.h"
#include "camera.h"

namespace givr {
namespace camera {
    constexpr const float LONGITUDE_MAX = 2. * M_PI;
    constexpr const float LATITUDE_MAX = M_PI;

    struct TurnTableCamera
        : public Projection<Longitude, Latitude, Zoom, Translation>
    {
        // TODO: ensure these are in the right ranges.
        TurnTableCamera() {
            set(Latitude(LATITUDE_MAX / 2.));
            set(Longitude(0.f));
            set(Zoom(75.f));
            set(Translation(0.f, 0.f, 0.f));
        }

        float const &latitude() const { return value<Latitude>().value(); }
        float const &longitude() const { return value<Longitude>().value(); }
        float const &zoom() const { return value<Zoom>().value(); }
        vec3f const &translation() const { return value<Translation>().value(); }

        float &latitude() { return value<Latitude>().value(); }
        float &longitude() { return value<Longitude>().value(); }
        float &zoom() { return value<Zoom>().value(); }
        vec3f &translation() { return value<Translation>().value(); }

        void rotateAroundXPercent(float perc);
        void rotateAroundYPercent(float perc);
        void rotateAroundX(float angle);
        void rotateAroundY(float angle);
        void zoom(float amount);
        void translate(vec3f amount);

        mat4f viewMatrix() const;
        vec3f viewPosition() const;
    };

    template <typename... Args>
    TurnTableCamera TurnTable(Args &&... args) {
        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(
            is_subset_of<std::tuple<Args...>, TurnTableCamera::Args> &&
            sizeof...(args) <= std::tuple_size<TurnTableCamera::Args>::value,
            "You have provided incorrect parameters for TurnTable. "
            "Longitude, Latitude, Zoom and Translation are optional.");
        TurnTableCamera pv;

        if constexpr (sizeof...(args) > 0) {
            pv.set(std::forward<Args>(args)...);
        }
        return pv;
    }

} // end namespace camera
} // end namespace givr
