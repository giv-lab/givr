#include "turntable.h"

#include <cmath>
#include <iostream>

using turntable = givr::turntable;

float angle_to_x(float r, float theta, float phi) {
    return r * sin(theta) * sin(phi);
}
float angle_to_y(float r, float phi) {
    return r * cos(phi);
}
float angle_to_z(float r, float theta, float phi) {
    return r * cos(theta) * sin(phi);
}


mat4f givr::get_view_matrix(turntable const & t) {
    vec3f camera_position{
        angle_to_x(t.radius, t.longitude, t.latitude),
        angle_to_y(t.radius, t.latitude),
        angle_to_z(t.radius, t.longitude, t.latitude)
    };
    camera_position += t.translation;
    vec3f up{ 0., 1., 0. };
    vec3f binormal = cross(camera_position, up);
    up = glm::normalize(cross(binormal, camera_position));

    return lookAt(
        camera_position,
        t.translation,
        // TODO: Not sure this up vector is correct
        up
    );
}

void turntable::rotate_around_x_percent(float perc) {
    rotate_around_x(perc * LONGITUDE_MAX);
}
void turntable::rotate_around_y_percent(float perc) {
    rotate_around_y(perc * LATITUDE_MAX);
}

void turntable::rotate_around_x(float angle) {
    longitude += angle;
}
void turntable::rotate_around_y(float angle) {
    latitude += angle;
}
void turntable::zoom(float amount) {
    radius += amount;
    radius = std::fmax(0, radius);
}
void turntable::translate(vec3f amount) {
    translation += amount;
}

