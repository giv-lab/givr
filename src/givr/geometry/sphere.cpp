#include "sphere.h"

#include <cassert>
#include <cmath>

using sphere = givr::sphere;

sphere::data givr::generate_geometry(sphere const &s) {
    sphere::data data;

    // TODO: should make this reserve the space necessary for the sphere.

    struct point { float v[3]; };
    //Azimuth: 0 to 2pi, Altitude: 0 to pi
    auto sphereFunc = [](float azi, float alt) {
        return point{ std::cos(azi)*std::sin(alt), std::cos(alt), std::sin(azi)*std::sin(alt)};
    };

    for (size_t azi = 0; azi < s.azimuthPoints; azi++) {
        for (size_t alt = 0; alt < s.altitudePoints; alt++) {
            if (azi < s.azimuthPoints - 1) {
                if (alt < s.altitudePoints - 1) {
                    data.indices.push_back(alt*s.azimuthPoints + azi);
                    data.indices.push_back((alt + 1)*s.azimuthPoints + azi);
                    data.indices.push_back((alt + 1)*s.azimuthPoints + azi + 1);

                    data.indices.push_back(alt*s.azimuthPoints + azi);
                    data.indices.push_back((alt + 1)*s.azimuthPoints + azi + 1);
                    data.indices.push_back(alt*s.azimuthPoints + azi + 1);
                }

            }
            else {
                data.indices.push_back(alt*s.azimuthPoints + azi);
                data.indices.push_back((alt + 1)*s.azimuthPoints + azi);
                data.indices.push_back((alt + 1)*s.azimuthPoints);

                data.indices.push_back(alt*s.azimuthPoints + azi);
                data.indices.push_back((alt + 1)*s.azimuthPoints);
                data.indices.push_back(alt*s.azimuthPoints);
            }
            float u = float(azi) / float(s.azimuthPoints - 1);
            float v = float(alt) / float(s.altitudePoints - 1);
            point spherePoint = sphereFunc(2.f*M_PI*u, M_PI*v);
            data.vertices.push_back(spherePoint.v[0]);
            data.vertices.push_back(spherePoint.v[1]);
            data.vertices.push_back(spherePoint.v[2]);

            data.normals.push_back(spherePoint.v[0]);
            data.normals.push_back(spherePoint.v[1]);
            data.normals.push_back(spherePoint.v[2]);
        }
    }

    assert(data.vertices.size() == data.normals.size());

    return data;
}

