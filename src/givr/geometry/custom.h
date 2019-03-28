#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <utility>
#include <gsl/span>

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

            gsl::span<const float> vertices;
            gsl::span<const float> normals;
            gsl::span<const uint32_t> indices;
            gsl::span<const float> colours;
            gsl::span<const float> uvs;
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
        data.vertices = gsl::span<const float>(
            reinterpret_cast<float const *>(l.vertices.data()),
            l.vertices.size()*3
        );
        data.normals = gsl::span<const float>(
            reinterpret_cast<float const *>(l.normals.data()),
            l.normals.size()*3
        );
        data.colours = gsl::span<const float>(
            reinterpret_cast<float const *>(l.colours.data()),
            l.colours.size()*3
        );
        data.uvs = gsl::span<const float>(
            reinterpret_cast<float const *>(l.uvs.data()),
            l.uvs.size()*2
        );
        data.indices = gsl::span<const std::uint32_t>(l.indices);

        return data;
    }

}// end namespace geometry
}// end namespace givr
