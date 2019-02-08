#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    template <PrimitiveType PrimitiveT>
    struct CustomGeometry {
        std::vector<vec3f> vertices;
        std::vector<vec3f> normals;
        std::vector<std::uint32_t> indices;
        std::vector<vec3f> colours;
        std::vector<vec2f> uvs;

        // TODO: add simpler ways to construct this.

        struct Data : public VertextArrayData<PrimitiveT> {
            BufferUsageType verticesType;
            BufferUsageType normalsType;
            BufferUsageType indicesType;
            BufferUsageType coloursType;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> colours;
            std::vector<float> uvs;
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

        __customGeometryCopy<vec3f, 3>(l.vertices, data.vertices);
        __customGeometryCopy<vec3f, 3>(l.normals, data.normals);
        __customGeometryCopy<vec3f, 3>(l.colours, data.colours);
        __customGeometryCopy<vec2f, 2>(l.uvs, data.uvs);

        data.indices.insert(data.indices.end(), l.indices.begin(), l.indices.end());

        return data;
    }

};// end namespace givr
