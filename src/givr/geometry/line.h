#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"
#include "parameters.h"

namespace givr {
namespace geometry {

    struct Line
        : public Geometry<Point1, Point2>
    {
        Line() {
        }

        template <typename... Args>
        Line(Args &&... args) {
            using required_args = std::tuple<Point1, Point2>;

            using namespace utility;
            static_assert(!has_duplicate_types<Args...>,
                "The arguments you passed in have duplicate parameters");

            static_assert(is_subset_of<required_args, std::tuple<Args...>>,
                "Point1 and Point2 are required parameters for Line. "
                "Please provide them.");
            static_assert(is_subset_of<std::tuple<Args...>, Line::Args>,
                "You have provided incorrect parameters for Line. "
                "Point1 and Point2 are required.");
            static_assert(sizeof...(args) <= std::tuple_size<Line::Args>::value,
                "You have provided incorrect parameters for Line. "
                "Point1 and Point2 are required.");
            set(std::forward<Args>(args)...);
        }

        vec3f const &p1() const { return value<Point1>().value(); }
        vec3f const &p2() const { return value<Point2>().value(); }

        vec3f &p1() { return value<Point1>().value(); }
        vec3f &p2() { return value<Point2>().value(); }

        struct Data : public VertexArrayData<PrimitiveType::LINES> {
            std::uint16_t dimensions = 3;

            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;
        };
    };
    // Backwards Compatibility
    using LineGeometry = Line;

    Line::Data generateGeometry(Line const &l);
}// end namespace geometry
}// end namespace givr
