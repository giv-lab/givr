#include "sphere.h"

#include <cassert>
#include <cmath>

using SphereGeometry = givr::geometry::SphereGeometry;

SphereGeometry::Data givr::geometry::generateGeometry(SphereGeometry const &s) {
    SphereGeometry::Data data;

    // TODO: should make this reserve the space necessary for the sphere.

    struct point { float v[3]; };
    //Azimuth: 0 to 2pi, Altitude: 0 to pi
    auto sphereFunc = [](float azi, float alt) {
        return point{ std::cos(azi)*std::sin(alt), std::cos(alt), std::sin(azi)*std::sin(alt) };
    };
    auto azimuthPoints = s.value<AzimuthPoints>().value();
    auto altitudePoints = s.value<AltitudePoints>().value();
    auto centroid = s.value<Centroid>().value();
    auto radius = s.value<Radius>().value();

    for (size_t azi = 0; azi < azimuthPoints; azi++) {
        for (size_t alt = 0; alt < altitudePoints; alt++) {
            if (azi < azimuthPoints - 1 && alt < altitudePoints - 1) {
                data.indices.push_back((alt + 1) + (azi + 1)* altitudePoints);
                data.indices.push_back((alt + 1) + azi * altitudePoints);
                data.indices.push_back(alt + azi * altitudePoints);

                data.indices.push_back(alt + (azi + 1)*altitudePoints);
                data.indices.push_back((alt + 1) + (azi + 1)*altitudePoints);
                data.indices.push_back(alt + azi * altitudePoints);
            }
            else if(alt < altitudePoints - 1){
                data.indices.push_back((alt + 1));
                data.indices.push_back((alt + 1) + azi * altitudePoints);
                data.indices.push_back(alt + azi * altitudePoints);

                data.indices.push_back(alt);
                data.indices.push_back((alt + 1));
                data.indices.push_back(alt + azi * altitudePoints);
            }
            float u = float(azi) / float(azimuthPoints - 1);
            float v = float(alt) / float(altitudePoints - 1);
            //Make uniform distribution
            v = acos(1 - 2.f*v) / M_PI;     //DELETE THIS IF SPHERES LOOK BAD

            point spherePoint = sphereFunc(2.f*M_PI*u, M_PI*v);
            data.vertices.push_back((radius * spherePoint.v[0]) + centroid[0]);
            data.vertices.push_back((radius * spherePoint.v[1]) + centroid[1]);
            data.vertices.push_back((radius * spherePoint.v[2]) + centroid[2]);

            data.normals.push_back(spherePoint.v[0]);
            data.normals.push_back(spherePoint.v[1]);
            data.normals.push_back(spherePoint.v[2]);

            data.uvs.push_back(u);
            data.uvs.push_back(v);
        }
    }

    assert(data.vertices.size() == data.normals.size());

    return data;
}
