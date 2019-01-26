#pragma once
#include <type_traits>

namespace givr {

    // Checking for normals
    template <typename T, typename = int>
    struct has_normals : std::false_type { };
    template <typename T>
    struct has_normals <T, decltype((void) T::data::normals, 0)> : std::true_type { };

    // Checking for vertices
    template <typename T, typename = int>
    struct has_vertices : std::false_type { };
    template <typename T>
    struct has_vertices <T, decltype((void) T::data::vertices, 0)> : std::true_type { };

    // Checking for indices
    template <typename T, typename = int>
    struct has_indices : std::false_type { };
    template <typename T>
    struct has_indices <T, decltype((void) T::data::indices, 0)> : std::true_type { };

    // Checking for colours
    template <typename T, typename = int>
    struct has_colours : std::false_type { };
    template <typename T>
    struct has_colours <T, decltype((void) T::data::colours, 0)> : std::true_type { };

    // Checking for uvs
    template <typename T, typename = int>
    struct has_uvs : std::false_type { };
    template <typename T>
    struct has_uvs <T, decltype((void) T::data::uvs, 0)> : std::true_type { };
};
