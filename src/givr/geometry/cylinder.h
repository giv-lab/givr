#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {
namespace geometry {

    struct CylinderGeometry
        : public Geometry<Point1, Point2, Radius, AzimuthPoints>
    {
        vec3f const &p1() const { return value<Point1>().value(); }
        vec3f const &p2() const { return value<Point2>().value(); }

        vec3f &p1() { return value<Point1>().value(); }
        vec3f &p2() { return value<Point2>().value(); }

        float radius() const { return value<Radius>().value(); }
        std::size_t azimuthPoints() const { return value<AzimuthPoints>().value(); }

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;

            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;
            std::vector<std::uint32_t> indices;
        };
    };

    template <typename... Args>
    CylinderGeometry Cylinder(Args &&... args) {
        using required_args = std::tuple<Point1, Point2>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<required_args, std::tuple<Args...>>,
            "Point1 and Point2 are a required parameter for Cylinder. "
            "Please provide them.");
        static_assert(is_subset_of<std::tuple<Args...>, CylinderGeometry::Args>,
            "You have provided incorrect parameters for Cylinder. "
            "Point1 and Point2 are required. Radius and AzimuthPoints are optional.");
        static_assert(sizeof...(args) <= std::tuple_size<CylinderGeometry::Args>::value,
            "You have provided incorrect parameters for Cylinder. "
            "Point1 and Point2 are required. Radius and AzimuthPoints are optional.");
        CylinderGeometry c;
        c.set(Radius(1.0f));
        c.set(AzimuthPoints(20));
        c.set(std::forward<Args>(args)...);
        return c;
    }

    CylinderGeometry::Data generateGeometry(CylinderGeometry const &l);
}// end namespace geometry
}// end namespace givr
