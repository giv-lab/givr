#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "triangle.h"

namespace givr {
namespace geometry {

    struct TriangleSoup {
        private:
            std::vector<TriangleGeometry> m_triangles;
        public:
            template <typename... Args>
            TriangleSoup(Args &&... args) {
                std::vector<TriangleGeometry> tris{args...};
                for (auto &t : tris) {
                    m_triangles.push_back(t);
                }
            }

            std::vector<TriangleGeometry> &triangles() { return m_triangles; }
            std::vector<TriangleGeometry> const &triangles() const { return m_triangles; }

            void push_back(TriangleGeometry t) {
                m_triangles.push_back(t);
            }
            void push_back(vec3f const &p1, vec3f const &p2, vec3f const &p3) {
                m_triangles.push_back(Triangle(Point1(p1), Point2(p2), Point3(p3)));
            }

            struct Data : public VertexArrayData<PrimitiveType::TRIANGLES> {
                std::uint16_t dimensions = 3;

                BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
                std::vector<float> vertices;

                BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
                std::vector<float> normals;
            };
    };
    // Backwards Compatibility
    using TriangleSoupGeometry = TriangleSoup;


    TriangleSoup::Data generateGeometry(TriangleSoup const &t);
}// end namespace geometry
}// end namespace givr
