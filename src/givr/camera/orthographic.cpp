#include "orthographic.h"

#include "../types.h"
#include <glm/gtc/matrix_transform.hpp>

using OrthographicProjection = givr::camera::OrthographicProjection;
using mat4f = givr::mat4f;

mat4f OrthographicProjection::projectionMatrix() const {
    // TODO: This assumes a view centred on the origin
    return glm::ortho(
        left() * aspectRatio(),
        right() * aspectRatio(),
        bottom(),
        top(),
        nearDistance(),
        farDistance()
    );
}
void OrthographicProjection::updateAspectRatio(int width, int height) {
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    set(AspectRatio(w / h));
}
