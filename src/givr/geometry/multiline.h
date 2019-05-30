#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "geometry.h"
#include "line.h"

namespace givr {
namespace geometry {

    struct MultiLine {
        private:
            std::vector<Line> m_segments;

        public:
            template <typename... Args>
            MultiLine(
                Args &&... args
            ) : m_segments{std::forward<Args>(args)...}
            { }

            std::vector<Line> &segments() { return m_segments; }
            std::vector<Line> const &segments() const { return m_segments; }
            std::vector<Line> &operator*() { return m_segments; }

            void push_back(Line l);

            struct Data : public VertexArrayData<PrimitiveType::LINES> {
                std::uint16_t dimensions = 3;

                BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
                std::vector<float> vertices;
            };
    };
    // Backwards Compatibility
    using MultiLineGeometry = MultiLine;


    MultiLine::Data generateGeometry(MultiLine const &l);
}// end namespace geometry
}// end namespace givr
