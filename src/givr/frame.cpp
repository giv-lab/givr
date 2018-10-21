#include "frame.h"

using frame = givr::frame;

frame givr::at(float x, float y) {
    return at(vec3f(x, y, 0.f));
}
frame givr::at(float x, float y, frame const &f) {
    return at(vec3f(x, y, 0.f), f);
}
frame givr::at(float x, float y, float z) {
    return at(vec3f(x, y, z));
}
frame givr::at(float x, float y, float z, frame const &f) {
    return at(vec3f(x, y, z), f);
}
frame givr::at(vec3f pos) {
    return at(pos, identity());
}
frame givr::at(vec3f pos, frame const &f) {
    return translate(f, pos);
}

frame givr::scale(float x, float y, float z) {
    return scale(x, y, z, identity());
}
frame givr::scale(float x, float y, float z, frame const &f) {
    return scale(f, vec3f(x, y, z));
}

