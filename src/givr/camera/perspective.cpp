#include "perspective.h"

#include "../types.h"
#include <glm/gtc/matrix_transform.hpp>

using PerspectiveProjection = givr::camera::PerspectiveProjection;

givr::mat4f PerspectiveProjection::projectionMatrix() const {
    return glm::perspective(
        glm::radians(fieldOfViewY()),
        aspectRatio(),
        nearDistance(),
        farDistance()
    );
}

void PerspectiveProjection::updateAspectRatio(int width, int height) {
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    set(AspectRatio(w / h));
}
