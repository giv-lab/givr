#include "../utility.h"
#include "../types.h"

namespace givr {
namespace camera {
    template <typename... Types>
    struct Camera
        : public utility::TypedStruct<Types...> {
    };
} // end namespace geometry
} // end namespace givr
