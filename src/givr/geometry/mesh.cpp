#include "mesh.h"
#include "tiny_obj_loader.h"
#include <iostream>
#include <unordered_map>
#include <tuple>

struct index_pair {
    unsigned int a, b;

    bool operator==(const index_pair& rh) const {
        return a == rh.a && b == rh.b;
    }

    bool operator<(index_pair right_hand) {
        return (a < right_hand.a) ? true : b < right_hand.b;
    }
};

namespace std {
    template<>
    struct hash<index_pair> {
        size_t operator()(const index_pair &key) const {
            return size_t(key.a << 16) | size_t(key.b);
        }
    };
};

namespace givr {
namespace geometry {

    template<typename V1, typename V2>
    std::tuple<std::vector<unsigned int>, V1, V2> two_index_unifier(
        const std::vector<unsigned int> &indices_A, const V1 &data_A, size_t components_A,
        const std::vector<unsigned int> &indices_B, const V2 &data_B, size_t components_B)
    {

        std::vector<unsigned int> unified_indices;
        V1 unified_dataA;
        V2 unified_dataB;
        std::unordered_map<index_pair, unsigned int> index_map;

        for (std::size_t i = 0; i < indices_A.size(); i++) {
            if (index_map.find({ indices_A[i], indices_B[i] }) == index_map.end()) {
                index_map[{indices_A[i], indices_B[i]}] = unified_dataA.size()/components_A;
                unified_indices.push_back(unified_dataA.size()/components_A);

                for (std::size_t j = 0; j < components_A; j++)
                    unified_dataA.push_back(data_A[indices_A[i]*components_A + j]);

                for (std::size_t j = 0; j < components_B; j++)
                    unified_dataB.push_back(data_B[indices_B[i]*components_B + j]);
            }
            else {
                unified_indices.push_back(index_map[{indices_A[i], indices_B[i]}]);
            }
        }

        return { unified_indices, unified_dataA, unified_dataB };
    }


    template<typename V1, typename V2, typename V3>
    std::tuple<std::vector<unsigned int>, V1, V2, V3> three_index_unifier(
        const std::vector<unsigned int> &indices_A, const V1 &data_A, size_t components_A,
        const std::vector<unsigned int> &indices_B, const V2 &data_B, size_t components_B,
        const std::vector<unsigned int> &indices_C, const V3 &data_C, size_t components_C)
    {

        std::vector<unsigned int> partially_unified_indices;
        V1 partially_unified_dataA;
        V2 partially_unified_dataB;
        V3 partially_unified_dataC;
        std::unordered_map<index_pair, unsigned int> partially_unified_index_map;

        for (std::size_t i = 0; i < indices_A.size(); i++) {
            if (partially_unified_index_map.find({ indices_A[i], indices_B[i] }) == partially_unified_index_map.end()) {
                partially_unified_index_map[ {indices_A[i], indices_B[i]} ] = partially_unified_dataA.size()/components_A;
                partially_unified_indices.push_back(partially_unified_dataA.size()/components_A);

                for(std::size_t j=0; j<components_A; j++)
                    partially_unified_dataA.push_back(data_A[indices_A[i]*components_A + j]);
                for (std::size_t j = 0; j<components_B; j++)
                    partially_unified_dataB.push_back(data_B[indices_B[i]*components_B + j]);
            }
            else {
                partially_unified_indices.push_back(partially_unified_index_map[{indices_A[i], indices_B[i]}]);
            }
        }

        std::vector<unsigned int> unified_indices;
        V1 unified_dataA;
        V2 unified_dataB;
        V3 unified_dataC;
        std::unordered_map<index_pair, unsigned int> unified_index_map;

        for (std::size_t i = 0; i < partially_unified_indices.size(); i++) {
            if (unified_index_map.find({ partially_unified_indices[i], indices_C[i] }) == unified_index_map.end()) {
                unified_index_map[{partially_unified_indices[i], indices_C[i] }] = unified_dataA.size()/components_A;
                unified_indices.push_back(unified_dataA.size()/components_A);

                for (std::size_t j = 0; j<components_A; j++)
                    unified_dataA.push_back(partially_unified_dataA[partially_unified_indices[i] * components_A + j]);
                for (std::size_t j = 0; j<components_B; j++)
                    unified_dataB.push_back(partially_unified_dataB[partially_unified_indices[i] * components_B + j]);
                for (std::size_t j = 0; j<components_C; j++)
                    unified_dataC.push_back(data_C[indices_C[i] * components_C + j]);
            }
            else {
                unified_indices.push_back(unified_index_map[{ partially_unified_indices[i], indices_C[i] }]);
            }
        }

        return { unified_indices, unified_dataA, unified_dataB, unified_dataC };
    }

    MeshGeometry::Data loadMeshFile(const char *file_name) {

        //Tiny obj loading
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string errors;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &errors, file_name)) {
            std::cerr << errors << std::endl;
            return MeshGeometry::Data{};
        }

        std::vector<float> multi_index_vertex_data = attrib.vertices;
        std::vector<float> multi_index_uv_data = attrib.texcoords;
        std::vector<float> multi_index_normal_data = attrib.normals;
        std::vector<unsigned int> vertex_indices;
        std::vector<unsigned int> uv_indices;
        std::vector<unsigned int> normal_indices;
        for (auto index : shapes[0].mesh.indices) {
            vertex_indices.push_back(index.vertex_index);
            uv_indices.push_back(index.texcoord_index);
            normal_indices.push_back(index.normal_index);
        }

        MeshGeometry::Data unifiedIndexMesh;

        if (multi_index_uv_data.size() == 0 && multi_index_normal_data.size() == 0) {
            unifiedIndexMesh.indices = vertex_indices;
            unifiedIndexMesh.vertices = multi_index_vertex_data;
        }
        else if (multi_index_normal_data.size() != 0 && multi_index_uv_data.size() == 0) {
            auto[unified_indices, unified_vertices, unified_normals] = two_index_unifier(
                vertex_indices, multi_index_vertex_data, 3,
                normal_indices, multi_index_normal_data, 3
            );

            unifiedIndexMesh.indices = unified_indices;
            unifiedIndexMesh.vertices = unified_vertices;
            unifiedIndexMesh.normals = unified_normals;
        }
        else if (multi_index_normal_data.size() == 0 && multi_index_uv_data.size() != 0) {
            auto[unified_indices, unified_vertices, unified_uvs] = two_index_unifier(
                vertex_indices, multi_index_vertex_data, 3,
                normal_indices, multi_index_uv_data, 2
            );

            unifiedIndexMesh.indices = unified_indices;
            unifiedIndexMesh.vertices = unified_vertices;
            unifiedIndexMesh.uvs = unified_uvs;
        }
        else{
            auto[unified_indices, unified_vertices, unified_normals, unified_uvs] = three_index_unifier(
                vertex_indices, multi_index_vertex_data, 3,
                normal_indices, multi_index_normal_data, 3,
                uv_indices, multi_index_uv_data, 2);

            unifiedIndexMesh.indices = unified_indices;
            unifiedIndexMesh.vertices = unified_vertices;
            unifiedIndexMesh.normals = unified_normals;
            unifiedIndexMesh.uvs = unified_uvs;
        }

        //unifiedIndexMesh.uvs.resize(unifiedIndexMesh.vertices.size() * 2 / 3);
        return unifiedIndexMesh;

    }

    MeshGeometry::Data generateGeometry(const MeshGeometry& m) {
        return loadMeshFile(m.value<Filename>().value().c_str());
    }

}// namespace geometry
}// namespace givr
