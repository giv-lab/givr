#pragma once
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"
#include "triangle.h"

namespace givr {

    struct TriangleSoup {
        std::vector<Triangle> triangles;

        void addLine(Triangle t) {
            triangles.push_back(t);
        }
        void addLine(vec3f const &p1, vec3f const &p2, vec3f const &p3) {
            triangles.push_back(Triangle{p1, p2, p3});
        }

        struct Data : public VertextArrayData<PrimitiveType::TRIANGLES> {
            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            std::vector<float> vertices;

            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            std::vector<float> normals;
        };
    };

    TriangleSoup::Data generateGeometry(TriangleSoup const &t);
};// end namespace givr
