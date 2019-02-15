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
    using Zoom = utility::Type<float, struct Zoom_Tag>;
    using Translation = utility::Type<vec3f, struct Translation_Tag>;
    using Left = utility::Type<float, struct Left_Tag>;
    using Right = utility::Type<float, struct Right_Tag>;
    using Bottom = utility::Type<float, struct Bottom_Tag>;
    using Top = utility::Type<float, struct Top_Tag>;

} // end namespace geometry
} // end namespace givr
