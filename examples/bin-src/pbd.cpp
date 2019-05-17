#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

#include "io.h"
#include "turntable_controls.h"

using namespace givr;
using namespace givr::camera;
using namespace givr::style;
using namespace givr::geometry;

#define   N                100    // number of particles
#define   BowlRadius       15.0f  // circular / spherical bowl radius

#define   X_DISPERSE       1.f    // range of particles dispersal in X
#define   Z_DISPERSE       0.f    // range of particles dispersal in Z
#define   Rmin             0.3f   // minimum particle radius
#define   Rmax             1.0f   // maximum particle radius
#define   SEED             1      // maximum particle radius

// Filling up the bowl
#define   VelocityDamping  0.98f
#define   Bounce           1.00f  // normal velocity transfer at bounce
#define   Slide            0.50f  // tangential velocity transfer at bounce
#define   ReleaseInterval  40.0f  // time interval (steps) between particle release

InstancedRenderContext<Sphere, Phong> spheres;

vec3f GravityDT(0, -0.005, 0); // change of velocity due to gravity per time step
vec3f BowlCenter(0, 0, 0);

// Find new velocity after bouncing off the bowl
vec3f Reflect (vec3f v, vec3f normal, float bounce, float slide)
{
    normal = glm::normalize(normal); // normal to the bowl at the point of contact
    normal *= v * normal; // normal component of velocity
    return -bounce * normal + slide * (v - normal);
}

struct ParticleInfo {
    vec3f p;      // position;
    vec3f v;      // velocity;
    float r;    // radius;
    float m;    // mass;
    int c  ;    // colour;
};

ParticleInfo particle[N];

int n;          // number of particles formed so far
float age;      // time from creating previous particle

void CreateParticles()
{
    if (n >= N)
        return;
    age += 1.0;
    if (age < ReleaseInterval)
        return;
    vec3f p(0, 2*BowlRadius, 0), v(0, 0, 0);
    p.x = X_DISPERSE * ((float) rand()) / RAND_MAX;
    p.z = Z_DISPERSE * ((float) rand()) / RAND_MAX;
    particle[n].p = p;
    particle[n].v = v;
    particle[n].r = Rmin + (Rmax-Rmin) * ((float) rand()) / RAND_MAX;
    particle[n].m = pow(particle[n].r, 3.0);
    particle[n].c = 1 + 5 * ((float) rand()) / RAND_MAX;
    ++n;
    age -= ReleaseInterval;
}


void DetectCollisions()
{
    int i, j;
    vec3f PosDiff;
    vec3f Displacement;
    float d, r_sum, m_sum;
    for (i = 0; i < n; ++i) {
        for (j = i+1; j < n; ++j) {
            r_sum = particle[i].r + particle[j].r;
            PosDiff = particle[j].p - particle[i].p;
            d = glm::length(PosDiff);
            if (d < r_sum) {
                Displacement = (r_sum-d)*glm::normalize(PosDiff);
                m_sum = particle[i].m + particle[j].m;
                particle[i].v -= Displacement * particle[j].m / m_sum;
                particle[j].v += Displacement * particle[i].m / m_sum;
            }
        }
    }
}

void MoveParticles()
{
    int i;
    vec3f tentative_p;

    for(i = 0; i < n; ++i) {
        particle[i].v += GravityDT;
        tentative_p = particle[i].p + particle[i].v;
        if (glm::length(tentative_p-BowlCenter) + particle[i].r < BowlRadius)
            particle[i].p = tentative_p;
        else {
            particle[i].v = Reflect(particle[i].v,
                BowlCenter - particle[i].p, Bounce, Slide);
            particle[i].p += particle[i].v;
            particle[i].p = glm::normalize(particle[i].p - BowlCenter)
                * (BowlRadius - particle[i].r);
        }
        particle[i].v *= VelocityDamping;
    }
}

void init() {
    srand(SEED);
    n = 0;
    age = ReleaseInterval;

    spheres = createInstancedRenderable(
        Sphere(),
        Phong(
            Colour(1.0, 1.0, 0.129),
            LightPosition(100., 100., 100.)
        )
    );
}

void step() {
    CreateParticles();
    DetectCollisions();
    MoveParticles();
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
        for (int i = 0; i < n; ++i) {
            auto m = translate(mat4f{1.}, particle[i].p);
            m = scale(m, vec3f{particle[i].r});
            addInstance(spheres, m);
        }
        draw(spheres, view);
    });
    exit(EXIT_SUCCESS);
}
