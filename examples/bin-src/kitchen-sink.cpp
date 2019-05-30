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

    auto phongStyle = Phong(
        LightPosition(0.0, 0.0, 100.0),
        Colour(1.0, 1.0, 0.1529)
    );
    auto instancedSpheres = createInstancedRenderable(Sphere(), phongStyle);
    auto sphere = createRenderable(Sphere(), phongStyle);
    auto palmTree = Mesh(Filename("./models/Palm_Tree.obj"));
    auto instancedTrees = createInstancedRenderable(palmTree, phongStyle);
    auto tree = createRenderable(palmTree, phongStyle);

    auto noshading = NoShading(Colour(1.0, 0.0, 0.0));
    auto flatSphere = createRenderable(Sphere(), noshading);
    auto instancedFlatSpheres = createInstancedRenderable(Sphere(), noshading);
    auto polyline = PolyLine<PrimitiveType::LINE_LOOP>(
        Point(-10.f, -10.f, 0.f),
        Point(10.f, -10.f, 0.f),
        Point(10.f, 10.f, 0.f),
        Point(-10.f, 10.f, 0.f),
        Point(-10.f, -10.f, 0.)
    );
    auto lineStyle = GL_Line(Width(15.), Colour(0.0, 1.0, 0.0));
    auto polylines = createRenderable(polyline, lineStyle);
    auto instancedPolylines = createInstancedRenderable(polyline, lineStyle);

    auto line = Line(
        Point1(-15.0, -15.0, 0.0),
        Point2(15.0, 15.0, 0.0)
    );
    auto lines = createRenderable(line, lineStyle);

    auto triangle = Triangle(
            Point1(0.0, 15.0, -10.0),
            Point2(-15.0, -15.0, -10.0),
            Point3(15.0, -15.0, -10.0)
    );
    auto triangles = createRenderable(triangle, phongStyle);

    auto multiLine = MultiLine(
        Line(Point1(-20.0, -20.0, 0.0), Point2(-20.0, -10.0, 0.0)),
        Line(Point1(-10.0, -10.0, 0.0), Point2(-10.0, 0.0, 0.0)),
        Line(Point1(0.0, 0.0, 0.0), Point2(0.0, 10.0, 0.0)),
        Line(Point1(10.0, 10.0, 0.0), Point2(10.0, 20.0, 0.0))
    );

    auto multilines = createRenderable(multiLine, lineStyle);

    auto cylinder = Cylinder(
        Point1(-15.0, 15.0, 0.f),
        Point2(-15.0, -15.0, 0.f)
    );
    auto cylinders = createRenderable(cylinder, phongStyle);

    glClearColor(1.f, 1.f, 1.f, 1.f);

    float u = 0.0;
    float v = 0.0;

    window.run([&](float /*frameTime*/) {
        view.projection.updateAspectRatio(window.width(), window.height());

        mat4f m = translate(mat4f{1.f}, vec3f{0., 5.0, 0.});
        addInstance(instancedSpheres, m);
        draw(instancedSpheres, view);

        float x = 40.0 * cos(u*0.1);
        float y = 40.0 * sin(u*0.1);
        phongStyle.set(LightPosition(x, y, 2.0));
        updateRenderable(Sphere(), phongStyle, sphere);
        updateRenderable(Sphere(), phongStyle, instancedSpheres);
        updateRenderable(palmTree, phongStyle, tree);
        updateRenderable(palmTree, phongStyle, instancedTrees);

        x = 10.0 * cos(u);
        y = 10.0 * sin(u);
        u += 0.05;
        m = translate(mat4f{1.f}, vec3f{x, y, 0.});
        draw(sphere, view, m);

        x = 3.0 * cos(v) + x;
        y = 3.0 * sin(v) + y;
        v += 0.1;
        m = translate(mat4f{1.f}, vec3f{x, y, 0.});
        draw(sphere, view, m);


        m = translate(mat4f{1.f}, vec3f{-5., -5.0, -5.});
        addInstance(instancedSpheres, m);
        draw(instancedSpheres, view);

        float angle = 365*sin(u*.001);
        x = 24.0 * cos(u*0.1);
        y = 24.0 * sin(u*0.1);
        u += 0.05;
        m = rotate(scale(translate(mat4f{1.f}, vec3f{x, y, 0.}), vec3f{4.f, 4.f, 4.f}), angle, vec3f{1.0f, 1.0f, 0.2f});
        draw(tree, view, m);

        x = 8.0 * cos(v*0.5) + x;
        y = 8.0 * sin(v*0.5) + y;
        v += 0.1;
        m = translate(mat4f{1.f}, vec3f{x, y, 0.});
        draw(tree, view, m);

        float r = 0.5 + (0.5 * cos(u*0.3));
        float g = 0.5 + (0.5 * sin(u*0.3));
        noshading.set(Colour(r, g, 0.0));
        updateRenderable(Sphere(), noshading, flatSphere);
        updateRenderable(Sphere(), noshading, instancedFlatSpheres);

        m = scale(mat4{1.f}, vec3f{4.f, 4.f, 4.f});
        m = translate(m, vec3f{0.f, -5.0, 0.});
        addInstance(instancedFlatSpheres, m);
        draw(instancedFlatSpheres, view);

        m = translate(m, vec3f{0.f, 5.0, 0.});
        draw(flatSphere, view, m);

        lineStyle.set(Width(15.0 * sin(u)));
        updateRenderable(polyline, lineStyle, polylines);
        updateRenderable(polyline, lineStyle, instancedPolylines);
        draw(polylines, view);

        m = rotate(
            translate(mat4f{1.f}, vec3f{15.0, -15.0, 0.}),
            angle,
            vec3f{0.f, 0.f, 1.f}
        );
        addInstance(instancedPolylines, m);
        draw(instancedPolylines, view);

        m = rotate(
            translate(mat4f{1.f}, vec3f{-15.0, 15.0, 0.}),
            angle,
            vec3f{0.f, 0.f, 1.f}
        );
        addInstance(instancedPolylines, m);
        draw(instancedPolylines, view);

        m = rotate(mat4f{1.f}, angle, vec3f{0.f, 0.f, 1.f});
        draw(lines, view, m);

        m = rotate(mat4f{1.f}, angle, vec3f{1.f, 0.f, 0.f});
        draw(triangles, view, m);

        m = rotate(mat4f{1.f}, angle, vec3f{0.f, 0.f, 1.f});
        draw(multilines, view, m);

        m = rotate(mat4f{1.f}, angle, vec3f{0.f, 0.f, 1.f});
        draw(cylinders, view, m);
    });
    exit(EXIT_SUCCESS);
}
