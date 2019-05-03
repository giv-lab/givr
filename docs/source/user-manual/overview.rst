Overview
==========

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

