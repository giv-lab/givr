#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"
#include "parameters.h"

namespace givr {
namespace geometry {

    struct Triangle
        : public Geometry<Point1, Point2, Point3>
    {
        template <typename... Args>
        Triangle(Args &&... args) {
            using required_args = std::tuple<Point1, Point2, Point3>;

            using namespace utility;
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");

            static_assert(
                is_subset_of<required_args, std::tuple<Args...>> &&
                is_subset_of<std::tuple<Args...>, Triangle::Args> &&
                sizeof...(args) <= std::tuple_size<Triangle::Args>::value,
                "You have provided incorrect parameters for Triangle. "
                "Point1, Point2 and Point3 are required.");
            set(std::forward<Args>(args)...);
        }
        vec3f const &p1() const { return value<Point1>().value(); }
        vec3f const &p2() const { return value<Point2>().value(); }
        vec3f const &p3() const { return value<Point3>().value(); }

        vec3f &p1() { return value<Point1>().value(); }
        vec3f &p2() { return value<Point2>().value(); }
        vec3f &p3() { return value<Point3>().value(); }

        struct Data : public VertexArrayData<PrimitiveType::TRIANGLES> {
            std::uint16_t dimensions = 3;
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;
        };
    };
    // Backwards Compatibility
    using TriangleGeometry = Triangle;


    Triangle::Data generateGeometry(Triangle const &t);
}// end namespace geometry
}// end namespace givr
