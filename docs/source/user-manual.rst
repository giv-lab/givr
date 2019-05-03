Building the Examples
=====================
.. highlight:: bash

There are some simple examples you can use to get started with givr.
Included within them are all necessary dependencies to compile except
for a compiler and CMake.

You will need a compiler capable of compiling C++17. We have tested these
projects on various recent distributions and it compiles and runs.

Download a recent version of the examples. The most recent version is:
https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-examples/-/archive/v0.0.8/givr-examples-v0.0.8.zip
Unzip this file somewhere.  Then continue to your OS specific instructions.

You can also use git to grab a copy, the gitlab repository is here:
https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-examples/tags/v0.0.8


Windows
-------
On windows, you will need Visual Studio 2017. You may also need to update it after installation so that your compiler is up to date (launch Visual Studio and click on the flag in the top corner).  You will also need to
download and install cmake: https://cmake.org/download/

Next to the directory that was created when you unzipped the above file,
create a :code:`build` directory.

Now run the cmake-gui command from your start menu. It requires two directories
the first is the directory which contains the CMakeLists.txt from the file you
unzipped above.  The second is the  :code:`build` directory that you created.

Once these have been selected, you can press the configure button. It will
run for a few seconds, produce some output in the bottom frame and then show
you some configuration entries that you can change if you would like. You do
not need to change any of these, despite the fact that they are coloured red.
Next, press the generate button. Finally, press the Open Project button.

Note that by default the project will not have a default startup file set. You
can choose one of the example programs as your default and then build and run it.

If you want more information on how to use cmake-gui on windows they have
instructions here: https://cmake.org/runningcmake/


Linux
-----
In linux you will need to install cmake. All modern distributions come
with a way to install it that is pretty simple or straightforward. In
Ubuntu you can use::

   sudo apt-get install cmake build-essential

In arch linux you can use::

   pacman -Sy cmake

Once cmake is installed, navigate to the directory which contains the
`CMakeLists.txt` file and run these commands::

    cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
    cmake --build build

This will create a `build` directory and build the project within it.
You can then run the examples afterwards with::

   ./build/sph
   ./build/pbd

OSX
---
I don't have a mac on which to test these instructions, so I'm going to depend
on someone else to figure this out and tell me what they are. Ostensibly, you
should be able to use the same commands as the linux version to build.

Starter Project
===============
There is a starter project you can use to get yourself started.
It contains the dependencies and a sample :code:`main.cpp` which
shows a spinning triangle.  

The build instructions for this start project are the same as for
the examples below, and have been ommitted.

You can find the latest release of the start project here:
https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-simple-project/-/archive/v0.0.8/givr-simple-project-v0.0.8.zip

Example program
===============
.. highlight:: c++

Here is a very simple program that renders a rotating triangle. We will use
this example to get a basic idea of what givr does. This example also uses
Here is very simple program that renders a rotating triangle. It uses the
`io.h` library that Andrew Owens created for managing GLFW, and the turntable
controls from the givr-examples. If you successfully built the examples,
you can follow along by opening triangle.cpp::

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
        // Set up your windowing system / OpenGL context 
        io::GLFWContext windows;
        auto window = windows.create(io::Window::dimensions{640, 480}, "Simple example");

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

Overview
=========

.. highlight:: cpp

givr provides a simple and safe API for rendering geometry. The following
gives you an overview of how you can use givr to draw things.

There are 8 things that need to be in place for givr to render things to
the screen:

 1. Include givr.h
 2. Using Namespace
 3. Instantiate camera/view information
 4. Instantiate your geometry
 5. Instantiate your style
 6. Create the renderable
 7. (Optional) Add instances
 8. Draw


1. Include givr.h
-----------------
Just like all C++ libraries, you must include it before you use it::

    #include <givr.h>

The triangle example also includes glm (for doing math), io.h (for handling
windowing; it essentially wraps GLFW), and turntable_controls.h (for
interacting with the scene, e.g. rotating and zooming in/out).

2. Using Namespace
--------------------------
givr uses namespaces to organize its code. In most of the examples
we make use of using namespace directives to shorten the amount of
code we have to type. How much you use this is up to you::

    using namespace glm;
    using namespace givr;
    using namespace givr::camera;
    using namespace givr::geometry;
    using namespace givr::style;

3. Instantiate Camera/View
--------------------------
givr comes with a builtin camera and projection class::

    auto view = View(TurnTable(), Perspective());

When your window changes size, you will want to inform the projection class
of the change in aspect ratio. For example, we are using GLFW and `io.h`::

    io::GLFWContext windows;
    auto window = windows.create(io::Window::dimensions{640,480}, “Simple example”);
    view.projection.updateAspectRatio(window.width(), window.height());

If you are using just GLFW directly::

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    view.projection.updateAspectRatio(width, height);

4. Instantiate Geometry
-----------------------
givr comes with a number of different types of geometry, e.g. lines, triangles, spheres,
a Mesh loaded from an OBJ file, and custom geometry.  

Note that when you instantiate the geometry object, you are not actually
building the geometry. It isn't until you create the renderable that the
geometry is created. In our triangle example code, we’ve basically rolled
steps 4-6 (instantiating geometry, instantiating style, and creating the
renderable) into one call::

        auto triangle = createRenderable(
            Triangle(Point1(0.0, 1., 0.), Point2(-1., -1., 0.), Point3(1., -1., 0.)),
            Phong(Colour(1., 1., 0.1529), LightPosition(2., 2., 15.))
        );

To instantiate the triangle only::

   auto triangle = Triangle(Point1(0.0, 1., 0.), Point2(-1., -1., 0.), Point3(1., -1., 0.));


See :ref:`givr-geometry` for more details on all of the types of geometry
that are supported.

5. Instantiate Style
--------------------
givr comes with two different styles: a smooth shaded phong style and a
line style for rendering lines. We saw above how the style instantiation
was included in the createRenderable call, but we could also instantiate it
separately. For example, Phong style instantiation could look like::

    auto phongStyle = Phong(
         Colour(1.0, 1.0, 0.1529),
         LightPosition(2.0, 2.0, 15.0)
     );

See :ref:`givr-styles` for more details on all of the types of styles
that are supported.

6. Create the renderable
------------------------
There are two types of renderables in givr: instanced and non-instanced.
Instanced geometry is used when you need to render many of the same object in a scene
where the only difference is the position and orientation of those objects
(for example, you could be drawing many balls falling into a bowl -- see
the example `pbd`).  Non-instanced geometry is slightly easier to use,
but requires a draw call for each instance.

We have already seen an example of creating the non-instanced renderable::

        auto triangle = createRenderable(
            Triangle(Point1(0.0, 1., 0.), Point2(-1., -1., 0.), Point3(1., -1., 0.)),
            Phong(Colour(1., 1., 0.1529), LightPosition(2., 2., 15.))
        );


An example of creating the instanced renderable::

    auto instancedSpheres = createInstancedRenderable(Sphere(), phongStyle);

7. (Optional) Add instances
---------------------------
If you are using the instanced renderable, then you must add individual
instances using the `addInstance` function.  It takes the renderable as
the first parameter and a 4x4 model matrix as the second renderable. (For
a working example, refer to `pbd`.)

You can use glm matrix transform functions to instantiate the matrix:
https://glm.g-truc.net/0.9.2/api/a00245.html

Adding instances looks approximately like this::

    // Use GLM to translate to a specific location.
    mat4f m = translate(mat4f{1.f}, vec3f{0., 5.0, 0.});
    addInstance(instancedSpheres, m);

8. Draw
-------
When you are ready to draw, simply call the draw command. Please note that 
givr does not clear the screen for you. You should remember to clear the
screen yourself using something like::

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

The draw calls for both instanced and non-instanced renderables are nearly
identical with one minor difference.  The following syntax works with both::

    draw(instancedSpheres, view);

In this version you pass in only your renderable and the view you created
with the associated camera/projection objects. If your renderable is an
instanced renderable it will draw as many instances as you set up when you
called `addInstance`. If your renderable is not an instanced renderable
then it will draw a single instance with the identity matrix as the model
transform matrix. This usually places the geometry at the origin.

If you have a non-instanced renderable, there is an alternative form of the
draw command that you can use to place the object somewhere else. You can
pass a third parameter which is the model transformation matrix for this
particular draw call, which is what we see in our triangle example::


    mat4f m{1.f};
    // Update model matrix, m:
    u += frameTime;
    auto angle = 365.f*sin(u*.01f);
    m = rotate(m, angle, vec3f{1.0, 1.0, 0.0});
    auto size = cos(u*0.1f);
    m = scale(m, 15.f*vec3f{size});
    // Alternate call to draw:
    draw(triangle, view, m);


Once again you can use
the glm matrix transformation functions to construct the appropriate matrix.

