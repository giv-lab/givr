#pragma once

#include "givm.h"

namespace givr {

    // The framing for a particular draw call. Includes position, normal etc.
    // is the model matrix
    using frame = givm::mat4f;
    using vec3f = givm::vec3f;

    // Note that all methods have two forms, one which
    // turns an identify matrix into one that positions the element at
    // the given position, and one that modifies the existing matrix to
    // translate it to this position.

    // Construct a frame with position only.
    frame at(float x, float y);
    frame at(float x, float y, frame const &f);
    frame at(float x, float y, float z);
    frame at(float x, float y, float z, frame const &f);
    frame at(vec3f pos);
    frame at(vec3f pos, frame const &f);

    frame scale(float x, float y, float z);
    frame scale(float x, float y, float z, frame const &f);

};// end namespace givr
