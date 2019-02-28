#pragma once
#include "../utility.h"
#include "../types.h"

namespace givr {
namespace geometry {
    template <typename... Types>
    struct Geometry
        : public utility::TypedStruct<Types...> {
    };
} // end namespace geometry
} // end namespace givr
