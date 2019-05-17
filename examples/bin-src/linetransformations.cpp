#include "givr.h"
#include <glm/gtc/matrix_transform.hpp>
#include "io.h"
#include "turntable_controls.h"

using namespace glm;
using namespace givr;
using namespace givr::style;
using namespace givr::geometry;
using namespace givr::camera;

int main(void)
{
    io::GLFWContext windows;
    auto window =
        windows.create(io::Window::dimensions{640, 480}, "Simple example");
    // Note this has to be called after the window creation.
    window.enableVsync(true);

    auto view = View(TurnTable(), Perspective());
    TurnTableControls controls(window, view.camera);

    auto lineStyle = GL_Line(Width(15.), Colour(0.0, 0.0, 0.0));
    auto line = Line(
        Point1(-1.0, 0.0, 0.0),
        Point2(1.0, 0.0, 0.0)
    );
    auto lines = createRenderable(line, lineStyle);

    glClearColor(1.f, 1.f, 1.f, 1.f);

    float u = 0.0;

    window.run([&](float /*frameTime*/) {
        view.projection.updateAspectRatio(window.width(), window.height());

        auto m = mat4f{1.0f};
        m[0][0] = m[1][1] = m[2][2] = 10.0f*std::sin(u);
        draw(lines, view, m);

        float angle = 365*sin(u*0.001);
        m = rotate(m, angle, vec3f{0.f, 0.f, 1.f});
        draw(lines, view, m);

        float x = 10.0 * cos(u*0.001);
        float y = 10.0 * sin(u*0.001);
        m = translate(mat4f{1.0}, vec3f{x, y, 0.});
        draw(lines, view, m);

        u += 0.1;

    });
    exit(EXIT_SUCCESS);
}
