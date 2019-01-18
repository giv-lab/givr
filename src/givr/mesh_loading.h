#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include "types.h"

namespace givr {
    struct mesh {
        std::string filename;

        struct data {
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

    mesh::data generate_geometry(const mesh& m);

    /*struct mesh_geometry {
            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<std::uint32_t> indices;
            std::vector<float> colors;
            std::vector<float> uvs;
    };

    mesh_geometry load_mesh_file(const char *file_name);
    

    struct sphere {
        std::size_t azimuthPoints = 20;
        std::size_t altitudePoints = 20;

        struct data {
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


    sphere::data generate_geometry(sphere const &s);*/
};// end namespace givr
