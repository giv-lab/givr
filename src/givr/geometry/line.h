#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"
#include "parameters.h"

namespace givr {
namespace geometry {

    struct LineGeometry
        : public Geometry<Point1, Point2>
    {
        vec3f const &p1() const { return value<Point1>().value(); }
        vec3f const &p2() const { return value<Point2>().value(); }

        vec3f &p1() { return value<Point1>().value(); }
        vec3f &p2() { return value<Point2>().value(); }

        struct Data : public VertextArrayData<PrimitiveType::LINES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;
        };
    };

    template <typename... Args>
    LineGeometry Line(Args &&... args) {
        using required_args = std::tuple<Point1, Point2>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<required_args, std::tuple<Args...>>,
            "Point1 and Point2 are  a required parameter for Line. "
            "Please provide them.");
        static_assert(is_subset_of<std::tuple<Args...>, LineGeometry::Args>,
            "You have provided incorrect parameters for Line. "
            "Point1 and Point2 are required.");
        static_assert(sizeof...(args) <= std::tuple_size<LineGeometry::Args>::value,
            "You have provided incorrect parameters for Line. "
            "Point1 and Point2 are required.");
        LineGeometry l;
        l.set(std::forward<Args>(args)...);
        return l;
    }

    LineGeometry::Data generateGeometry(LineGeometry const &l);
}// end namespace geometry
}// end namespace givr
