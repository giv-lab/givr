#include "../utility.h"
#include "../types.h"

//------------------------------------------------------------------------------
// The base style command, which can do a few things.
//------------------------------------------------------------------------------

namespace givr {
namespace style {
    template <typename... Types>
    struct Style
        : public utility::TypedStruct<Types...> {
    };
} // end namespace style
} // end namespace givr
