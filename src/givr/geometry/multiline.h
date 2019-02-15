#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"
#include "line.h"

namespace givr {
namespace geometry {

    struct MultiLineGeometry {
        private:
            std::vector<LineGeometry> m_segments;

        public:
            std::vector<LineGeometry> &segments() { return m_segments; }
            std::vector<LineGeometry> const &segments() const { return m_segments; }
            std::vector<LineGeometry> &operator*() { return m_segments; }

            void push_back(LineGeometry l);

            struct Data : public VertextArrayData<PrimitiveType::LINES> {
                BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
                std::vector<float> vertices;
            };
    };

    template <typename... Args>
    MultiLineGeometry MultiLine(Args &&... args) {
        MultiLineGeometry geometry;
        geometry.segments() = {args...};
        return geometry;
    }

    MultiLineGeometry::Data generateGeometry(MultiLineGeometry const &l);
}// end namespace geometry
}// end namespace givr
