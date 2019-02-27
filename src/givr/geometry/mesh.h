#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include "../types.h"
#include "../vertex_array_data.h"
#include "geometry.h"
#include "parameters.h"

namespace givr {
namespace geometry {
    struct MeshGeometry
        : public Geometry<Filename>
          // TODO: Add other parameters like smooth shading etc.
    {

        std::string const &filename() const { return value<Filename>().value(); }
        std::string &filename() { return value<Filename>().value(); }

        struct Data : VertextArrayData<PrimitiveType::TRIANGLES> {
            std::uint16_t dimensions = 3;

            BufferUsageType verticesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType normalsType = BufferUsageType::STATIC_DRAW;
            BufferUsageType indicesType = BufferUsageType::STATIC_DRAW;
            BufferUsageType uvsType = BufferUsageType::STATIC_DRAW;

            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> uvs;
        };
    };

    template <typename... Args>
    MeshGeometry Mesh(Args &&... args) {
        using required_args = std::tuple<Filename>;

        using namespace utility;
        static_assert(!has_duplicate_types<Args...>,
            "The arguments you passed in have duplicate parameters");

        static_assert(is_subset_of<required_args, std::tuple<Args...>>,
            "Filename is a required parameter for Mesh. "
            "Please provide them.");
        static_assert(is_subset_of<std::tuple<Args...>, MeshGeometry::Args>,
            "You have provided incorrect parameters for Mesh. "
            "Filename is required.");
        static_assert(sizeof...(args) <= std::tuple_size<MeshGeometry::Args>::value,
            "You have provided incorrect parameters for Mesh. "
            "Filename is required.");
        MeshGeometry m;
        m.set(std::forward<Args>(args)...);
        return m;
    }

    MeshGeometry::Data generateGeometry(const MeshGeometry& m);

}// end namespace geometry
}// end namespace givr
