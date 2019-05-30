//------------------------------------------------------------------------------
// A simple example showing how to use the triangle geometry
//------------------------------------------------------------------------------
#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>

#include "io.h"
#include "turntable_controls.h"

using namespace glm;
using namespace givr;
using namespace givr::camera;
using namespace givr::geometry;
using namespace givr::style;

int main(void)
{
    io::GLFWContext windows;
    auto window = windows.create(io::Window::dimensions{640, 480}, "Simple example");
    // Note this has to be called after the window creation.
    window.enableVsync(true);

    auto view = View(TurnTable(), Perspective());
    TurnTableControls controls(window, view.camera);

    auto triangle = createRenderable(
        Triangle(Point1(0.0, 1., 0.), Point2(-1., -1., 0.), Point3(1., -1., 0.)),
        Phong(Colour(1., 1., 0.1529), LightPosition(2., 2., 15.))
    );

    glClearColor(1.f, 1.f, 1.f, 1.f);
    float u = 0.;
    window.run([&](float frameTime) {
        view.projection.updateAspectRatio(window.width(), window.height());
        mat4f m{1.f};
        u += frameTime;
        auto angle = 365.f*sin(u*.01f);
        m = rotate(m, angle, vec3f{1.0, 1.0, 0.0});
        auto size = cos(u*0.1f);
        m = scale(m, 15.f*vec3f{size});
        draw(triangle, view, m);
    });
    exit(EXIT_SUCCESS);
}
