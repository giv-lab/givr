#pragma once
#include "../utility.h"
#include "../types.h"

namespace givr {
namespace style {
    template <typename... Types>
    struct Style
        : public utility::TypedStruct<Types...> {
    };
} // end namespace style
} // end namespace givr
