#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../types.h"
#include "../vertex_array_data.h"
#include "geometry.h"
#include "parameters.h"

namespace givr {
namespace geometry {

    struct Sphere
        : public Geometry<Centroid, Radius, AzimuthPoints, AltitudePoints>
    {
        template <typename... Args>
        Sphere(Args &&... args) {
            using namespace utility;
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");

            static_assert(is_subset_of<std::tuple<Args...>, Sphere::Args>,
                "You have provided incorrect parameters for Sphere. "
                "Centroid, Radius, AzimuthPoints, AltitudePoints are optional.");
            static_assert(sizeof...(args) <= std::tuple_size<Sphere::Args>::value,
                "You have provided incorrect parameters for Sphere. "
                "Centroid, Radius, AzimuthPoints, AltitudePoints are optional.");
            this->set(Centroid(0.0, 0.0, 0.0));
            this->set(Radius(1.0));
            this->set(AzimuthPoints(20.));
            this->set(AltitudePoints(20.));
            if constexpr (sizeof...(args) > 0) {
                this->set(std::forward<Args>(args)...);
            }
        }

        struct Data : public VertexArrayData<PrimitiveType::TRIANGLES> {
            std::uint16_t dimensions = 3;

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
    // Backwards Compatibility
    using SphereGeometry = Sphere;


    Sphere::Data generateGeometry(Sphere const &s);

}// end namespace geometry
}// end namespace givr
