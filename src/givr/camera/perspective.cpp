#include "perspective.h"

#include "../types.h"
#include <glm/gtc/matrix_transform.hpp>

using mat4f = givr::mat4f;

mat4f givr::getProjectionMatrix(PerspectiveView const & p) {
    return glm::perspective(glm::radians(p.fovy), p.aspect, p.nearDistance, p.farDistance);
}

void givr::PerspectiveView::updateAspectRatio(int width, int height) {
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    aspect = w / h;
}
