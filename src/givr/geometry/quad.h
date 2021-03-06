#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"
#include "parameters.h"

namespace givr {
namespace geometry {

    struct Quad
        : public Geometry<Point1, Point2, Point3, Point4>
    {
        template <typename... Args>
        Quad(Args &&... args) {
            using required_args = std::tuple<Point1, Point2, Point3>;

            using namespace utility;
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");
            static_assert(
                is_subset_of<required_args, std::tuple<Args...>> &&
                is_subset_of<std::tuple<Args...>, Quad::Args> &&
                sizeof...(args) <= std::tuple_size<Quad::Args>::value,
                "You have provided incorrect parameters for Quad. "
                "Point1, Point2, and Point3 are required. Point4 is optional. "
                "If you provide Point4, then you are responsible for ensuring it "
                "is co-planar with Point1, Point2, and Point3");
            if constexpr (sizeof...(args) > 0) {
                set(std::forward<Args>(args)...);
            }
            if constexpr (sizeof...(args) == 3) {
                set(Point4(p2() + (p3() - p1())));
            }
        }

        vec3f const &p1() const { return value<Point1>().value(); }
        vec3f const &p2() const { return value<Point2>().value(); }
        vec3f const &p3() const { return value<Point3>().value(); }
        vec3f const &p4() const { return value<Point4>().value(); }

        vec3f &p1() { return value<Point1>().value(); }
        vec3f &p2() { return value<Point2>().value(); }
        vec3f &p3() { return value<Point3>().value(); }
        vec3f &p4() { return value<Point4>().value(); }

        struct Data : public VertexArrayData<PrimitiveType::TRIANGLES> {
            std::uint16_t dimensions = 3;

            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;

            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;
            std::vector<std::uint32_t> indices;

            BufferUsageType uvsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> uvs;
        };
    };
    // Backwards compatibility
    using QuadGeometry = Quad;


    Quad::Data generateGeometry(Quad const &t);
}// end namespace geometry
}// end namespace givr
