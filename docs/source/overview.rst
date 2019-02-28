Overview
==========

.. highlight:: cpp

givr provides a simple and safe API for rendering geometry.  It does nothing
to setup your windowing system or OpenGL context. You must do these things
yourself.  In addition it doesn't do a couple of other things like clearing
the screen or setting the glViewport. You must still do those things on your
own. The following gives you an overview of how you use givr to draw things.

There are 5 things that need to be in place for givr to render things to
the screen:

 1. Include givr.h
 2. Instantiate camera/view information
 3. Using Namespace
 4. Instantiate your geometry
 5. Instantiate your style
 6. Create the renderable
 7. (Optional) Add instances
 8. Draw


1. Include givr.h
-----------------
Just like all C++ libraries, you must include it before you use it::

    #include <givr.h>

3. Using Namespace
--------------------------
givr uses namespaces to organize it's code. In most of the examples
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
of the change in aspect ratio. For example, if you are using GLFW::

     int width, height;
     glfwGetFramebufferSize(window, &width, &height);
     view.projection.updateAspectRatio(width, height);

4. Instantiate Geometry
-----------------------
givr comes with a number of different types of geometry.  For example a sphere,
a Mesh loaded from an OBJ file, lines, triangles and custom geometry.  

Note that when you instantiate the geometry object, you are not actually
building the geometry. It isn't until you create the renderable that the
geometry is created.

A sphere is instantiated like this::

   auto sphere = Sphere();

by default it is a unit sphere centred at the origin. See the geometry
documentation for how to change its parameters.

A Mesh is instantiated like this::

    auto palm_tree = Mesh("./models/Palm_Tree.obj");

meshes are placed wherever their obj file places them. Usually this is
centred at the origin, but it's not guaranteed.

Custom Geometry is instantiated like this::

    CustomGeometry customGeometry;
    customGeometry.vertices.push_back(vec3f{10.0, 10.0, 0.0});
    customGeometry.vertices.push_back(vec3f{-10.0, -10.0, 0.0});

custom geometry classes also have fields for normals, colours,
uv coordinates, and indices if this is indexed geometry.

See :ref:`givr-geometry` for more details on all of the types of geometry
that are supported.

5. Instantiate Style
--------------------
givr comes with two different styles. A smooth shaded phong style and a line
style for rendering lines.

Phong style instantiation::

    auto phongStyle = Phong(
         Colour(1.0, 1.0, 0.1529),
         LightPosition(2.0, 2.0, 15.0)
     );

See :ref:`givr-styles` for more details on all of the types of styles
that are supported.

6. Create the renderable
------------------------
There are two types of renderables in givr. Instanced and non-instanced.
Instanced geometry is used when you need to render many of the same object in a scene
where the only difference is the position and orientation of those objects.
Non-instanced geometry. Is slightly easier to use, but requires a draw call
for each instance.

An example of creating the non-instanced renderable::

    auto sphere = createRenderable(
        Sphere(),
        Phong(
            Colour(1.0, 1.0, 0.1529),
            LightPosition(2.0, 2.0, 15.0)
        )
    );

An example of creating the instanced renderable::

    auto instancedSpheres = createInstancedRenderable(Sphere(), phongStyle);

7. (Optional) Add instances
---------------------------
If you are using the instanced renderable, then you must add individual
instances using the `addInstance` function.  It takes the renderable as
the first parameter and a 4x4 model matrix as the second renderable.

You can use glm matrix transform functions to instantiate the matrix:
https://glm.g-truc.net/0.9.2/api/a00245.html

Adding instances looks approximately like this::

    // Use GLM TO translate to a specific location.
    mat4f m = translate(mat4f{1.f}, vec3f{0., 5.0, 0.});
    addInstance(instancedSpheres, m);

8. Draw
-------
When you are ready to draw, simply call the draw command. As a reminder,
givr does not clear the screen for you. You should remember to clear the
screen yourself using something like::

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

The draw calls for both instanced and non-instanced renderables are nearly
identical with one minor difference.  The following syntax works with both::

    draw(instancedSpheres, view);

In this version you pass in only your renderable and the view you created
with the associated camera/projection objects. If your renderable is an
instanced renderable it will draw as many instances as you setup when you
called `addInstance`. If your renderable is not an instanced renderable
then it will draw a single instance with the identity matrix as the model
transform matrix. This usually places the geometry at the origin.

If you have a non-instanced renderable there is an alternative form of the
draw command that you can use to place the object somewhere else::

    mat4f m = translate(mat4f{1.f}, particle.position);
    draw(sphere, view, m);

In this version, you can pass a third parameter which is the model
transformation matrix for this particular draw call. Once again you can use
the glm matrix transformation functions to construct the appropriate matrix.


