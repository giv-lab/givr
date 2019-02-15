#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../types.h"
#include "../vertex_array_data.h"
#include "geometry.h"

namespace givr {
namespace geometry {

    struct SphereGeometry
        : public Geometry<Centroid, Radius, AzimuthPoints, AltitudePoints>
    {

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType uvsType = BufferUsageType::STATIC_DRAW;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> uvs;
        };

    };

    template <typename... Args>
    SphereGeometry Sphere(Args &&... args) {
        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<std::tuple<Args...>, SphereGeometry::Args>,
            "You have provided incorrect parameters for Sphere. "
            "Centroid, Radius, AzimuthPoints, AltitudePoints are optional.");
        static_assert(sizeof...(args) <= std::tuple_size<SphereGeometry::Args>::value,
            "You have provided incorrect parameters for Sphere. "
            "Centroid, Radius, AzimuthPoints, AltitudePoints are optional.");
        SphereGeometry s;// = Sphere();
        s.set(Centroid(0.0, 0.0, 0.0));
        s.set(Radius(1.0));
        s.set(AzimuthPoints(20.));
        s.set(AltitudePoints(20.));
        if constexpr (sizeof...(args) > 0) {
            s.set(std::forward<Args>(args)...);
        }
        return s;
    }

    SphereGeometry::Data generateGeometry(SphereGeometry const &s);

}// end namespace geometry
}// end namespace givr
