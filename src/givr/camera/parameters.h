#include "../utility.h"
#include "../types.h"
#include <string>

//------------------------------------------------------------------------------
// Defines all of the parameters to camera functions.
//
// Uses custom types for each name so that we can pass them in as "named"
// parameters.
//------------------------------------------------------------------------------

namespace givr {
namespace camera {

    using FieldOfViewY = utility::Type<float, struct FieldOfViewY_Tag>;
    using AspectRatio = utility::Type<float, struct AspectRatio_Tag>;
    using NearDistance = utility::Type<float, struct NearDistance_Tag>;
    using FarDistance = utility::Type<float, struct FarDistance_Tag>;
    using Latitude = utility::Type<float, struct Latitude_Tag>;
    using Longitude = utility::Type<float, struct Longitude_Tag>;

} // end namespace geometry
} // end namespace givr
