#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

#include "../vertex_array_data.h"
#include "../types.h"

namespace givr {

    template <primitive_type PrimitiveT>
    struct custom_geometry {
        std::vector<vec3f> vertices;
        std::vector<vec3f> normals;
        std::vector<std::uint32_t> indices;
        std::vector<vec3f> colours;
        std::vector<vec2f> uvs;

        // TODO: add simpler ways to construct this.

        struct data : public vertex_array_data<PrimitiveT> {
            buffer_usage_type vertices_type;
            buffer_usage_type normals_type;
            buffer_usage_type indices_type;
            buffer_usage_type colours_type;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> colours;
            std::vector<float> uvs;
        };

    };

    template<typename VectorT, int vecsize>
    void __custom_geometry_copy(std::vector<VectorT> const &source, std::vector<float> &target) {
        for (std::size_t i = 0; i < source.size(); ++i) {
            for (std::size_t j = 0; j < vecsize; ++j) {
                target.push_back(source[i][j]);
            }
        }
    };

    template <primitive_type PrimitiveT>
    typename custom_geometry<PrimitiveT>::data generate_geometry(custom_geometry<PrimitiveT> const &l) {
        typename custom_geometry<PrimitiveT>::data data;

        __custom_geometry_copy<vec3f, 3>(l.vertices, data.vertices);
        __custom_geometry_copy<vec3f, 3>(l.normals, data.normals);
        __custom_geometry_copy<vec3f, 3>(l.colours, data.colours);
        __custom_geometry_copy<vec2f, 2>(l.uvs, data.uvs);

        data.indices.insert(data.indices.end(), l.indices.begin(), l.indices.end());

        return data;
    }

};// end namespace givr
