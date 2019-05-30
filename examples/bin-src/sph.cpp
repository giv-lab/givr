#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

#include "io.h"
#include "turntable_controls.h"

using namespace givr;
using namespace givr::camera;
using namespace givr::style;
using namespace givr::geometry;

#define TwoPi 6.28318530717959f

#define N       400       // total number of particles
#define M       20        // number of columns of particles
#define sigma2  0.42f     // controls smoothing distance

#define dt      1.0f      // time step

InstancedRenderContext<Sphere, Phong> spheres;

using V2f = vec3f;

struct particle_structure {
    V2f p;
    V2f v;
    V2f a;
    float dens;
    float pres;
} sph_particle [N];

float kernel(V2f p) {
    float normalized_distance2 = (p.x*p.x + p.y*p.y) / (2.0*sigma2);
    if (normalized_distance2 > 50)
        return 0;
    else
        return (1.0 / (TwoPi*sigma2)) * exp(-normalized_distance2);
}

V2f kernel_gradient(V2f p) {
    V2f temp(p.x, p.y, 0.f);
    return (-temp / sigma2) * kernel(p);
}

float density(int i) {
    float result = 0;
    for(int j=0; j<N; ++j) {
        result += kernel(sph_particle[i].p-sph_particle[j].p);
    }
    return result;
}

float pressure(int i) {
    return 0.01 * pow(sph_particle[i].dens - 0.05, 9.0);
}

V2f accel_due_to_pressure(int i) {
    V2f result(0.f, 0.f, 0.f);
    for(int j=0; j<N; j++) {
        result += kernel_gradient(sph_particle[j].p - sph_particle[i].p) *
            (sph_particle[j].pres + sph_particle[i].pres) / sph_particle[j].dens;
    }
    return result / sph_particle[i].dens;
}

V2f accel_due_to_viscosity(int i) {
    V2f result(0.f, 0.f, 0.f);
    for (int j=0; j<N; j++) {
        result += 0.1f * kernel(sph_particle[j].p - sph_particle[i].p) *
            (sph_particle[j].v - sph_particle[i].v) / sph_particle[j].dens;
    }
    return result / sph_particle[i].dens;
}

V2f gravity(0.0, -0.003, 0.f);

void init() {
    for (int i=0; i<N/M; i++) {
        for(int j = 0; j<M; j++) {
            sph_particle[M*i+j].p = V2f(2.0 + (float) i, 0.0 + (float) j, 0.f);
            sph_particle[M*i+j].v = V2f(0.0, 0.0, 0.f);
        }
    }
    spheres = createInstancedRenderable(
        Sphere(),
        Phong(
            Colour(0., 0., 1.),
            LightPosition(10., 10.0, 10.0)
        )
    );
}

void step() {
    for(int i=0; i<N; i++) {
        sph_particle[i].dens = density(i);
        sph_particle[i].pres = pressure(i);
    }
    for(int i=0; i<N; i++) {
        sph_particle[i].a = accel_due_to_pressure(i)
                        + accel_due_to_viscosity(i)
                        + gravity;
    }
    for(int i=0; i<N; i++) {
        sph_particle[i].v += sph_particle[i].a * dt;
        if(sph_particle[i].p.y <= -10) {
            sph_particle[i].v.y *= -0.5;
        }
        if(sph_particle[i].p.x <=-20 || sph_particle[i].p.x >= 20) {
            sph_particle[i].v.x *= -0.5;
        }
        sph_particle[i].p += sph_particle[i].v * dt;
    }
}

int main() {
    io::GLFWContext windows;
    auto window = windows.create(io::Window::dimensions{640, 480}, "Point Based Dynamics");
    // Note this has to be called after the window creation.
    window.enableVsync(true);

    auto view = View(TurnTable(), Perspective());
    TurnTableControls controls(window, view.camera);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    init();
    window.run([&](float) {
        step();
        view.projection.updateAspectRatio(window.width(), window.height());
        for (int i = 0; i < N; ++i) {
            auto m = translate(mat4f{1.f}, sph_particle[i].p);
            m = scale(m, vec3f{0.4f});
            addInstance(spheres, m);
        }
        draw(spheres, view);
    });
    exit(EXIT_SUCCESS);
}
