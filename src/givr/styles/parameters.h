#pragma once
#include "../utility.h"
#include "../types.h"

//------------------------------------------------------------------------------
// Defines all of the parameters to style functions.
//
// Uses custom types for each name so that we can pass them in as "named"
// parameters.
//------------------------------------------------------------------------------

namespace givr {
namespace style {

    using AmbientFactor = utility::Type<float, struct AmbientFactor_Tag>;
    using Colour = utility::Type<vec3f, struct Colour_Tag>;
    using LightPosition = utility::Type<vec3f, struct LightPosition_Tag>;
    using PerVertexColour = utility::Type<bool, struct PerVertexColor_Tag>;
    using PhongExponent = utility::Type<float, struct PhongExponent_Tag>;
    using SpecularFactor = utility::Type<float, struct SpecularFactor_Tag>;
    using Width = utility::Type<float, struct Width_Tag>;

} // end namespace style
} // end namespace givr
