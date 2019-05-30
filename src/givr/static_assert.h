#pragma once
#include <type_traits>

namespace givr {

    // Checking for normals
    template <typename T, typename = int>
    struct hasNormals : std::false_type { };
    template <typename T>
    struct hasNormals <T, decltype((void) T::Data::normals, 0)> : std::true_type { };

    // Checking for vertices
    template <typename T, typename = int>
    struct hasVertices : std::false_type { };
    template <typename T>
    struct hasVertices <T, decltype((void) T::Data::vertices, 0)> : std::true_type { };

    // Checking for indices
    template <typename T, typename = int>
    struct hasIndices : std::false_type { };
    template <typename T>
    struct hasIndices <T, decltype((void) T::Data::indices, 0)> : std::true_type { };

    // Checking for colours
    template <typename T, typename = int>
    struct hasColours : std::false_type { };
    template <typename T>
    struct hasColours <T, decltype((void) T::Data::colours, 0)> : std::true_type { };

    // Checking for uvs
    template <typename T, typename = int>
    struct hasUvs : std::false_type { };
    template <typename T>
    struct hasUvs <T, decltype((void) T::Data::uvs, 0)> : std::true_type { };
};
