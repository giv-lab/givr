#pragma once
#include "../utility.h"
#include "../types.h"
#include <string>

//------------------------------------------------------------------------------
// Defines all of the parameters to geometry functions.
//
// Uses custom types for each name so that we can pass them in as "named"
// parameters.
//------------------------------------------------------------------------------

namespace givr {
namespace geometry {

    using Point = utility::Type<vec3f, struct Point_Tag>;
    using Point1 = utility::Type<vec3f, struct Point1_Tag>;
    using Point2 = utility::Type<vec3f, struct Point2_Tag>;
    using Point3 = utility::Type<vec3f, struct Point3_Tag>;
    using Point4 = utility::Type<vec3f, struct Point4_Tag>;
    using Centroid = utility::Type<vec3f, struct Centroid_Tag>;
    using Radius = utility::Type<float, struct Point3_Tag>;
    using AzimuthPoints = utility::Type<std::size_t, struct AzimuthPoints_Tag>;
    using AltitudePoints = utility::Type<std::size_t, struct AltitudePoints_Tag>;
    using Filename = utility::Type<std::string, struct Point3_Tag>;

} // end namespace geometry
} // end namespace givr
