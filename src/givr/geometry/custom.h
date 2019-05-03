#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <utility>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {
namespace geometry {

    template <PrimitiveType PrimitiveT>
    struct CustomGeometry {
        std::vector<vec3f> vertices;
        std::vector<vec3f> normals;
        std::vector<std::uint32_t> indices;
        std::vector<vec3f> colours;
        std::vector<vec2f> uvs;

        // TODO: add simpler ways to construct this.

        struct Data : public VertextArrayData<PrimitiveT> {
            std::uint16_t dimensions = 3;

            BufferUsageType verticesType;
            BufferUsageType normalsType;
            BufferUsageType indicesType;
            BufferUsageType coloursType;

            std::pair<int, float const *> vertices;
            std::pair<int, float const *> normals;
            std::pair<int, std::uint32_t const *> indices;
            std::pair<int, float const *> colours;
            std::pair<int, float const *> uvs;
        };

    };

    template<typename VectorT, int vecsize>
    void __customGeometryCopy(std::vector<VectorT> const &source, std::vector<float> &target) {
        for (std::size_t i = 0; i < source.size(); ++i) {
            for (std::size_t j = 0; j < vecsize; ++j) {
                target.push_back(source[i][j]);
            }
        }
    };

    template <PrimitiveType PrimitiveT>
    typename CustomGeometry<PrimitiveT>::Data generateGeometry(CustomGeometry<PrimitiveT> const &l) {
        typename CustomGeometry<PrimitiveT>::Data data;
        data.vertices = std::make_pair(
            l.vertices.size()*3,
            reinterpret_cast<float const *>(l.vertices.data())
        );
        data.normals = std::make_pair(
            l.normals.size()*3,
            reinterpret_cast<float const *>(l.normals.data())
        );
        data.colours = std::make_pair(
            l.colours.size()*3,
            reinterpret_cast<float const *>(l.colours.data())
        );
        data.uvs = std::make_pair(
            l.uvs.size()*2,
            reinterpret_cast<float const *>(l.uvs.data())
        );
        data.indices = std::make_pair(l.indices.size(), l.indices.data());

        return data;
    }

}// end namespace geometry
}// end namespace givr
