Introduction
============
.. highlight:: c++

Your job as a 3D graphics programmer is to put pixels on the screen.
Figuring out how to do that with modern computer graphics pipelines like
OpenGL or Vulkan is a daunting and complicated task.  Given the plethora of
concepts, data structures, api calls and opaque state that you must deal
with, the complexity can be bewildering.  However, the resulting programs
are some of the most fun and rewarding experiences that you can have as a
programmer.

When you first start out, you may not realize how many concepts are necessary
to get a fully functional graphics program running. This introduction will
cover all of this and provide you with some example libraries and code that 
you can use to get started.

A graphics program will typically need to do all of the following:

   * Initialize a window using the user interface libraries for the operating system
   * Initialize communication with the graphics card drivers
   * Setup OpenGL (or Vulkan or Direct3D) appropriately in preparation for rendering
   * Handle keyboard and mouse input from the user
   * Load the resources you will use for rendering (models, textures, etc).
   * Upload these resources to the GPU
   * Provide programmable instructions to the GPU on how to render those resources
   * Integrate and use a system for displaying graphical widgets like dropdowns and buttons
   * Math related utilities for vector, matrix and opengl related operations.

Thankfully you will rarely (if ever?) need to deal with each of these tasks directly.
You will almost always use a set of libraries which helps you deal with them and may even
provide a cross-platform API that you may use.  There are many options for libraries that 
deal with these requirements and I cannot list them all, however some of the ones I have
used and can recommend are:

  1. `Simple Direct Media Library (SDL) <https://www.libsdl.org/>`_:
     a very popular cross platform library for dealing with user input, window creation, OpenGL context setup
     image loading, text rendering, sound and many other items.
  2. `Graphics Library FrameWork (GLFW) <https://www.glfw.org/>`_:
     another popular cross platform library for dealing with user input, window creation and OpenGL context
     setup.
  3. `OpenGL Mathematics (GLM) <https://glm.g-truc.net/0.9.9/index.html>`_:
     A header only C++ mathematics library for graphics software which is based on the
     `OpenGL Shading Language Specifications <https://www.khronos.org/registry/OpenGL/index_gl.php>`_.
  4. `Eigen <http://eigen.tuxfamily.org/index.php?title=Main_Page>`_:
     Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and
     related algorithms.
  5. There are many. For a more comprehensive list see the
     `Khronos Related Toolkits and APIS <https://www.khronos.org/opengl/wiki/Related_toolkits_and_APIs>`_ page.

For windowing and OpenGL context management we will be using the excellent
`givio <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givio>`_
library written by Andrew Owens.  It wraps GLFW into a simple to use, C++-17 interface which greatly
simplifies its setup and usage.

For mathematics, we will be using the GLM library as it closely follows the GLSL specification which
reduces cognitive overhead when you switch between writing GLSL and C++.


The following code is an example of a program uses :code:`givr`, :code:`givio` and :code:`glm`. The rest of this introduction
will slowly build up to this final program, and each line will be explained.
 
.. :emphasize-lines: 4, 11, 12, 13, 14, 22, 25, 26, 27, 28, 33, 40

.. code-block:: c++
  :linenos:

    #include "givr.h"
    #include <glm/gtc/matrix_transform.hpp>

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


