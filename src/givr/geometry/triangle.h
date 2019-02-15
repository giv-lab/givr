#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"

namespace givr {
namespace geometry {

    struct TriangleGeometry
        : public Geometry<Point1, Point2, Point3>
    {
        vec3f const &p1() const { return value<Point1>().value(); }
        vec3f const &p2() const { return value<Point2>().value(); }
        vec3f const &p3() const { return value<Point3>().value(); }

        vec3f &p1() { return value<Point1>().value(); }
        vec3f &p2() { return value<Point2>().value(); }
        vec3f &p3() { return value<Point3>().value(); }

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;
        };
    };

    template <typename... Args>
    TriangleGeometry Triangle(Args &&... args) {
        using required_args = std::tuple<Point1, Point2, Point3>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<required_args, std::tuple<Args...>>,
            "Point1, Point2 and Point3 are  a required parameter for Triangle. "
            "Please provide them.");
        static_assert(is_subset_of<std::tuple<Args...>, TriangleGeometry::Args>,
            "You have provided incorrect parameters for Triangle. "
            "Point1, Point2 and Point3 are required.");
        static_assert(sizeof...(args) <= std::tuple_size<TriangleGeometry::Args>::value,
            "You have provided incorrect parameters for Triangle. "
            "Point1, Point2 and Point3 are required.");
        TriangleGeometry t;
        t.set(std::forward<Args>(args)...);
        return t;
    }

    TriangleGeometry::Data generateGeometry(TriangleGeometry const &t);
}// end namespace geometry
}// end namespace givr
