#include "turntable.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>

using mat4f = givr::mat4f;
using vec3f = givr::vec3f;
using TurnTable = givr::TurnTable;

float angle_to_x(float r, float theta, float phi) {
    return r * sin(theta) * sin(phi);
}
float angle_to_y(float r, float phi) {
    return r * cos(phi);
}
float angle_to_z(float r, float theta, float phi) {
    return r * cos(theta) * sin(phi);
}


mat4f givr::getViewMatrix(TurnTable const & t) {
    vec3f camera_position{
        angle_to_x(t.radius, t.longitude, t.latitude),
        angle_to_y(t.radius, t.latitude),
        angle_to_z(t.radius, t.longitude, t.latitude)
    };
    camera_position += t.translation;
    vec3f up{ 0., 1., 0. };
    vec3f binormal = cross(camera_position, up);
    up = glm::normalize(cross(binormal, camera_position));

    return glm::lookAt(camera_position, t.translation, up);
}

vec3f givr::getViewPosition(TurnTable const & t) {
    return vec3f{
        angle_to_x(t.radius, t.longitude, t.latitude),
        angle_to_y(t.radius, t.latitude),
        angle_to_z(t.radius, t.longitude, t.latitude) };
}

void TurnTable::rotateAroundXPercent(float perc) {
    rotateAroundX(perc * LONGITUDE_MAX);
}
void TurnTable::rotateAroundYPercent(float perc) {
    rotateAroundY(perc * LATITUDE_MAX);
}

void TurnTable::rotateAroundX(float angle) {
    longitude += angle;
}
void TurnTable::rotateAroundY(float angle) {
    latitude = std::fmin(std::fmax(latitude + angle, 0.001f), M_PI - 0.001f);
}
void TurnTable::zoom(float amount) {
    radius += amount;
    radius = std::fmax(0, radius);
}
void TurnTable::translate(vec3f amount) {
    translation += amount;
}

