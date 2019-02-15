#include "perspective.h"

#include "../types.h"
#include <glm/gtc/matrix_transform.hpp>

givr::mat4f givr::camera::getProjectionMatrix(PerspectiveViewProjection const & p) {
    return glm::perspective(
        glm::radians(p.fieldOfViewY()),
        p.aspectRatio(),
        p.nearDistance(),
        p.farDistance()
    );
}

void givr::camera::PerspectiveViewProjection::updateAspectRatio(int width, int height) {
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    set(AspectRatio(w / h));
}
