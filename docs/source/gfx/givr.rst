Rendering with `givr`
=====================
.. highlight:: c++

Once you have a window up and running, the next step is to render something.
This is where :code:`givr` comes in.  It provides you with a simple API
to quickly get pixels onto your screen. The following is a sample program which
renders a yellow triangle in the middle of the screen.

The new lines that have been added since the last program have been highlighted.
We will go through them one section at a time. It is worth noting that this point
that we will not be giving a thorough introduction to :code:`givr` as that 
is already covered in the user manual, but rather will be giving a cursory and
brief overview.

.. code-block:: c++
  :linenos:
  :emphasize-lines: 1, 5, 6, 7, 8, 9, 21, 23, 24, 25, 26, 28, 30, 31, 32

    #include "givr.h"

    #include "io.h"

    using namespace glm;
    using namespace givr;
    using namespace givr::camera;
    using namespace givr::geometry;
    using namespace givr::style;

    int main(void)
    {
        io::GLFWContext windows;
        auto window = windows.create(io::Window::dimensions{640, 480}, "Simple example");
        window.enableVsync(true);

        window.keyboardCommands()
          | io::Key(GLFW_KEY_ESCAPE,
              [&](auto const &/*event*/) { window.shouldClose(); });

        auto view = View(TurnTable(), Perspective());

        auto triangle = createRenderable(
            Triangle(Point1(0.0, 1., 0.), Point2(-1., -1., 0.), Point3(1., -1., 0.)),
            Phong(Colour(1., 1., 0.1529), LightPosition(2., 2., 15.))
        );

        glClearColor(1.f, 1.f, 1.f, 1.f);
        window.run([&](float frameTime) {
            view.projection.updateAspectRatio(window.width(), window.height());
            mat4f m{1.f};
            draw(triangle, view, m);
        });
        exit(EXIT_SUCCESS);
    }

Lines 1 and 5-9 (shown below) are how you include givr and some using namespace
statements to make the following code more succinct.

.. code-block:: c++

    #include "io.h"

    using namespace glm;
    using namespace givr;
    using namespace givr::camera;
    using namespace givr::geometry;
    using namespace givr::style;


Line 21 (show below) instantiates the view for the program. The view is composed of
two elements, the Camera and the Projection. We will not go into the details of how
each of these work, but it is necessary to have a view.

.. code-block:: c++

        auto view = View(TurnTable(), Perspective());

Lines 23-26 do a number of things at the same time. First, a triangle geometry is
instantiated and its points are specified. Second a Phong shading based style is
instantiated to render a yellow colour and with a light in given position.
Finally, both of these are combined into a renderable called `triangle`.
Again, we will not go into the details of these, but all three are required to
put a pixels onto the screen.

.. code-block:: c++

        auto triangle = createRenderable(
            Triangle(Point1(0.0, 1., 0.), Point2(-1., -1., 0.), Point3(1., -1., 0.)),
            Phong(Colour(1., 1., 0.1529), LightPosition(2., 2., 15.))
        );

Line 28 (shown below) is an opengl call to set the clear color. Givr does not
change this opengl state for you, so it's up to you to set it appropriately.

.. code-block:: c++

        glClearColor(1.f, 1.f, 1.f, 1.f);


Lines 30-32 are the lines which tell givr to render the triangle for each frame.
The first line updates the view based on the current window size (so that the view
changes when the user updates the window size). The second line instantiates a
an identity matrix and the last line tells givr to draw the triangle renderable
with the given view and model matrix (identity)

.. code-block:: c++

            view.projection.updateAspectRatio(window.width(), window.height());
            mat4f m{1.f};
            draw(triangle, view, m);


This is a minimal givr program which does very little and illustrates the simplicity
of getting pixels onto the screen with givr.
