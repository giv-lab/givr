#include "perspective.h"

#include "../types.h"
#include <glm/gtc/matrix_transform.hpp>

mat4f givr::get_projection_matrix(perspective_view const & p) {
    return glm::perspective(glm::radians(p.fovy), p.aspect, p.near_distance, p.far_distance);
}
void givr::perspective_view::update_aspect_ratio(int width, int height) {
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    aspect = w / h;
}
