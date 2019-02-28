.. _givr-geometry:
.. role:: cpp(code)
   :language: cpp

Geometry
==========

.. highlight:: cpp


Introduction
------------
The following documentation gives you a terse, but reasonably complete description
of the types of geometry that you can instantiate within givr.

**Note**: We use some Template Metaprogramming techniques to provide this API.

These techniques are used so that your code will not compile when the
you have provided incomplete data or incorrect types. It also allows you
to provide the parameters to each function call in whatever order you choose
and enforces a style that promotes readability.  The name of the struct
which constains the geometry has a different name from the function which
you use to instantiate it. As such we make liberal use of the keyword
:cpp:`auto` to simplify the examples.

If you are creating a simple demo in a single file and using global
variables to hold all of your data/styles/geometry then you can use the example
code below in the same format.

If you intend to organize your code into classes and need to know the exact
types that are used in order to declare member variables or function
parameters that take these as types, then read the *Advanced* section at the
end.


Types of Geometry
-----------------

givr provides the following types of geometry:

Triangle
--------------------------------------------------------------------------------

The Triangle geometry is used to create a single triangle.

Parameters
**********
It has three parameters, one for each point of the triangle:

 - :cpp:`Point1(float, float, float)`: The position of vertex 1

   - **Required**

 - :cpp:`Point2(float, float, float)`: The position of vertex 2

   - **Required**

 - :cpp:`Point3(float, float, float)`: The position of vertex3 

   - **Required**

Data
******

The triangle geometry provides the following data to the style:

  - vertices 
  - normal 

    -  **NOTE:** The normal is calculated as if the winding order is clockwise.

Example
********

::

    auto triangle = Triangle(
        Point1(0.f, 1.f, 0.f),
        Point2(-1.f, -1.f, 0.f),
        Point3(1.f, -1.f, 0.f)
    ),


Line
--------------------------------------------------------------------------------

The line geometry is for generating a single line segment.

Parameters
**********
It has two parameters, one for each point of the triangle:

 - :cpp:`Point1(float, float, float)`: The position of vertex 1

   - **Required**

 - :cpp:`Point2(float, float, float)`: The position of vertex 2

   - **Required**

Data
******
The line geometry provides this data to the style:
  * vertices 

Example
*******

::

    auto line = Line(
        Point1(-15.0, -15.0, 0.0),
        Point2(15.0, 15.0, 0.0)
    );


MultiLine
--------------------------------------------------------------------------------

A MultiLine is simply a series of line segments which may or may not
be connected. It is analogous to the GL_LINES rendering type.

Parameters
************
It takes an arbitrary number of lines as its parameters.

You can also add line segments using the following API::

    auto l = MultiLine();
    l.push_back(Line(Point1(-20.f, -20.f, 0.f), Point2(-20.f, -10.f, 0.f))));

Data
******
The MultiLine geometry provides this data to the style:
  * vertices 

Example
*******

::

    auto multiLine = MultiLine(
        Line(Point1(-20.f, -20.f, 0.f), Point2(-20.f, -10.f, 0.f)),
        Line(Point1(-10.f, -10.f, 0.f), Point2(-10.f, 0.f, 0.f)),
        Line(Point1(0.f, 0.f, 0.f), Point2(0.f, 10.f, 0.f)),
        Line(Point1(10.f, 10.f, 0.f), Point2(10.f, 20.f, 0.f))
    );

Polyline
--------------------------------------------------------------------------------

A polyline is composed of a series of points where each line segment connects
the current point with the previous point. It is valid for any number of
points greater than 1.

Parameters
************
It is a templated class, which takes givr::PrimitiveType as the template
parameter. This template parameter may be set to one of two values:

  - :cpp:`givr::PrimitiveType::LINE_LOOP`
  - :cpp:`givr::PrimitiveType::LINE_STRIP`

If you use :cpp:`PrimitiveType::LINE_LOOP`, the final point will be connected by a line segment with
the first point. If you use :code:`PrimitiveType::LINE_STRIP` then it will not be. This parameter
is a template parameter so that we can do compile time checking to ensure it is
set to the right value. 

The class takes a list of points as parameters.

Data
******
The PolyLine geometry provides this data to the style:
  * vertices 

Example
*******
::

    auto polyline = PolyLine<PrimitiveType::LINE_LOOP>(
        Point(-10.f, -10.f, 0.f),
        Point(10.f, -10.f, 0.f),
        Point(10.f, 10.f, 0.f),
        Point(-10.f, 10.f, 0.f),
        Point(-10.f, -10.f, 0.)
    );


Sphere
--------------------------------------------------------------------------------

The sphere geometry is used to generate a set of triangles which approximate
a sphere.  By default the sphere is a unit sphere, centred around the
origin. You can change where its :cpp:`Centroid` and its :cpp:`Radius` by
providing them when you construct it, or you can use a model matrix to place
it in the correct position and scale it appropriately.


Parameters
************
It has four parameters:

 - :cpp:`Centroid(float, float, float)`: The point around which the sphere is
   centred.

   - *Default*: :cpp:`Centroid(0.f, 0.f, 0.f)`

 - :cpp:`Radius(float)`: The radius of the sphere.

   - *Default*: :cpp:`Radius(1.f)`

 - :cpp:`AzimuthPoints(int)`: The number of azimuthal sample points to use.

   - *Default*: :cpp:`AzimuthPoints(20)`

 - :cpp:`AltitudePoints(int)`: The number of altitude sample points to use.

   - *Default*: :cpp:`AltitudePoints(20)`


Data
******
The sphere produces:
   - vertices
   - normals
   - indices
   - uvs 

*Note*: uv coordinates are not currently used by any styles.


Example
*******
Typically you will just use the sphere as is and scale it when you draw it::

    auto instancedSpheres = createInstancedRenderable(Sphere(), phongStyle);

    mat4f m = translate(mat4f{1.f}, vec3f{0., 5.0, 0.});
    addInstance(instancedSpheres, m);
    draw(instancedSpheres, view);

Alternatively, you can change its parameters directly when creating it::

    auto spheres = createRenderable(
        Sphere(
            Centroid(1.0f, -10.f, 0.f),
            Radius(5.f),
            AzimuthPoints(5),
            AltitudePoints(5)
        ),
        phongStyle
    );

    draw(spheres, view);


Cylinder
--------------------------------------------------------------------------------
The Cylinder geometry allows you to place a cylinder that connects two points.
It's often used in place of :cpp:`GL_LINES` as it is actually a 3D object, while
:cpp:`GL_LINES` are not.

**Note:** The current implementation is an open-faced cylinder.

Parameters
************
It has four parameters:

 - :cpp:`Point1(float, float, float)`: the first end point of the cylinder
   centred.

   - *Default*: :cpp:`Point1(0.f, 0.5f, 0.f)`

 - :cpp:`Point2(float, float, float)`: the first end point of the cylinder
   centred.

   - *Default*: :cpp:`Point1(0.f, -0.5f, 0.f)`

 - :cpp:`Radius(float)`: The radius of the cylinder.

   - *Default*: :cpp:`Radius(1.f)`

 - :cpp:`AzimuthPoints(int)`: The number of azimuthal sample points to use.

   - *Default*: :cpp:`AzimuthPoints(20)`


Data
******
It generates this data for the style to use:
   - vertices
   - normals
   - indices


Example
*******

::

    auto cylinder = Cylinder(
        Point1(-15.0, 15.0, 0.f),
        Point2(-15.0, -15.0, 0.f)
    );


Mesh
--------------------------------------------------------------------------------
The Mesh geometry allows you to load arbitrary meshes from .obj files and then
render them.

Parameters
************
It has a single parameter, which is the filename of the .obj. Note that it
attempts to load the filename you give it directly, without modification. This
means that it is your responsibility to ensure that the path will work when
your executable is run.  If you use relative paths, you will need to ensure
that your application is always run in the same directory. If you use absolute
paths then you will need to ensure there is a way to easily change that when
you move the program between machines:

 - :cpp:`Filename(std::string)`: The filename to load

   - **Required**

Data
******
The Mesh object will produce the following data for the style to use:
   - vertices
   - normals
   - indices
   - uvs

Example
*******
::

    auto palmTree = Mesh(Filename("./models/Palm_Tree.obj"));

Triangle Soup
--------------------------------------------------------------------------------
This is the first option for defining your own custom geometry. It's slightly
easier to use, but also slightly less efficient.

Triangle soup is an affectionate name for large set of triangles
representing an object, but no implicit connectivity or topology. This
geometry type is like the :cpp:`CustomGeometry` (described below) in that it
allows you to easily build new shapes surfaces or other items, but it provides
a slightly easier to use interface to do so.

*NOTE*: This type of geometry produces normals for each triangle, and assigns
that normal to each vertex of that triangle.  In addition, each vertex of the
triangle is explicitly represented in the vertices array regardless of whether
other triangles share the same vertex. The result of this is that they shading
will not be smooth across the edges of triangles. If you want custom geometry
with smooth shading, you will need to use givr::CustomGeometry (see below).

Parameters
************
It takes a list of triangles as its parameters.

You can also add triangles using the following API::

    auto ts = TriangleSoup();
    ts.push_back(
        Triangle(
            Point1(-20.f, -20.f, 0.f),
            Point2(-10.f, -10.f, 0.f),
            Point3(-20.f, 0.f, 0.f)
        )
    );

Data
******

The triangle geometry these pieces of data which are made available to the style:
  * vertices 
  * normals

Example
********
::

    auto ts = TriangleSoup(
        Triangle(
            Point1(-20.f, -20.f, 0.f),
            Point2(-10.f, -10.f, 0.f),
            Point3(-20.f, 0.f, 0.f)
        ),
        Triangle(
            Point1(-40.f, -40.f, 0.f),
            Point2(-20.f, -20.f, 0.f),
            Point3(-40.f, -10.f, 0.f)
        )
    );


Or more likely you will loop over the elements in your animation/simulation
and turn them into a series of triangles::

   auto ts = triangleSoup();
   // Loop over all objects in your simulation/animation
   for(int i = 0; i < my_simulation.objects.size(); ++i) {
      // Get a reference to the object
      object const &o = my_simulation.objects[i];

      // Turn that object into a Triangle (or triangles!)
      TriangleSoup t{o.get_point1(), o.get_point2(), o.get_point3()};

      // Add that triangle to the triangle soup
      ts.push_back(t);
   }

As a specific example, here is how I generated the triangles for the sides
of my jelly cube for the mass springs assignment.  I stored my particle masses
in a 1D vector, and then I painstakingly did all of the index math to generate
triangles. It wasn't fun, I'm sure there are better ways::

   auto jellyGeometry = TriangleSoup();

   void updateJellyGeometry() {
       // This gets called for every frame, so it's not hyper efficient, but
       // reasonable for 60ish fps
       jellyGeometry.triangles.clear();

       auto pos = [&](std::size_t i, std::size_t j, std::size_t k) {
           return jelly.particles[(i*(resolution*resolution)) + (j*resolution) + k].position;
       };
       auto addTriangle = [&](vec3f const &p1, vec3f const &p2, vec3f const &p3) {
           jellyGeometry.push_back(givr::Triangle{p1, p2, p3});
       };

       for (std::size_t i = 0; i < resolution; ++i) {
           for (std::size_t j = 0; j < resolution; ++j) {
               for (std::size_t k = 0; k < resolution; ++k) {
                   if (i == 0  && j!=0 && k!=0) {
                       addTriangle(pos(i, j-1, k-1), pos(i, j, k), pos(i, j, k-1));
                       addTriangle(pos(i, j-1, k-1), pos(i, j-1, k), pos(i, j, k));
                   }
                   if (i +1 == resolution  && j +1 != resolution && k != 0) {
                       addTriangle(pos(i, j+1, k-1), pos(i, j, k), pos(i, j, k-1));
                       addTriangle(pos(i, j+1, k-1), pos(i, j+1, k), pos(i, j, k));
                   }
                   if (j == 0  && i!=0 && k!=0) {
                       addTriangle(pos(i-1, j, k-1), pos(i, j, k), pos(i, j, k-1));
                       addTriangle(pos(i-1, j, k-1), pos(i-1, j, k), pos(i, j, k));
                   }
                   if (j +1 == resolution  && i +1 != resolution && k != 0) {
                       addTriangle(pos(i+1, j, k-1), pos(i, j, k), pos(i, j, k-1));
                       addTriangle(pos(i+1, j, k-1), pos(i+1, j, k), pos(i, j, k));
                   }
                   if (k == 0  && i!=0 && j!=0) {
                       addTriangle(pos(i-1, j-1, k), pos(i, j, k), pos(i, j-1, k));
                       addTriangle(pos(i-1, j-1, k), pos(i-1, j, k), pos(i, j, k));
                   }
                   if (k +1 == resolution  && i +1 != resolution && j != 0) {
                       addTriangle(pos(i+1, j-1, k), pos(i, j, k), pos(i, j-1, k));
                       addTriangle(pos(i+1, j-1, k), pos(i+1, j, k), pos(i, j, k));
                   }
               }
           }
       }
   };

Custom Geometry
--------------------------------------------------------------------------------
This type of geometry is here so that you can specify your own geometry. It is
quite flexible, with the caveat that you are required to understand how geometry
is typically provided to the GPU and manage all of the indices, vertices, normals
colours or uv coordinates yourself. It does very little compile time or run time
checking. As a result, you are responsible for all aspects of this particular
geometry. 

*NOTE:* The renderers that we use assume a few things about the setup of this data.

  - vertices are 3 floats. 
  - normals are 3 floats.
  - uvs are 2 floats
  - colours are 3 floats.
  - indices are 32 bit unsigned integers.

in order to enforce this convention, the parameters for custom geometry are
specified as `vec3fs` or `vec2fs` or single `std::uint32_t` for indices.

Also note, that the vertices, normals, uvs, and colours vector must either contain
0 elements or the same number of elements or you risk a segfault from within the
graphics driver.

Also note, that if you provide indices, it will be rendered as indexed geometry. If
you do not provide indices it will not be rendered as indexed geometry.

*NOTE*: None of the current styles use the uv coordinates.

Parameters
************
The `CustomGeometry` is a templated class, which takes givr::PrimitiveType
as the template parameter. This template parameter may be set to any of the
`givr::PrimitiveType` values::

    enum class PrimitiveType {
        POINTS,
        LINES,
        LINE_LOOP,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        LINES_ADJACENCY,
        LINE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY,
        TRIANGLE_STRIP_ADJACENCY
    };


The `CustomGeometry` class provides lists of `vec3f` for vertices, normals
and colours, a list of `vec2f` 
::

    template <PrimitiveType PrimitiveT>
    struct CustomGeometry {
        std::vector<vec3f> vertices;
        std::vector<vec3f> normals;
        std::vector<std::uint32_t> indices;
        std::vector<vec3f> colours;
        std::vector<vec2f> uvs;
    }

Data
******
It provides the data you provide to the style. 

Example
*******
No examples for this one. The primary reason is that I haven't written a good
example for this, but I'll also claim that if you're considering using this type
of geometry, then you should be willing to read an existing tutorial on how to
setup these sorts of buffers for rendering. The exact format depends on whether
it's indexed, which primitive type you are using etc.

Advanced
------------
As mentioned in the introduction, we use the C++ :cpp:`auto` keyword liberally
in the example code above. This hides the actual types that are used throughout.
This section explains the types a bit more concretely.

Named Parameters
****************
The various parameters that are passed into the geometry are an sub class of
the :cpp:`givr::utility::Type` class which is templated.  These classes wrap
some other type, like a :cpp:`glm::vec3` or a :cpp:`float`. Each of the sub
classes are named after the parameter they represent. 
Each of the instantiation functions for the Geometry operate on these named
types.
It is the usage of these named parameters which allows us to perform various
compile time checking to ensure your code is more likely to run correctly.
It also allows us to take the parameters for a given geometry out of order.
In other words, the following two examples are equivalent::

    auto line = Line(
        Point1(-15.0, -15.0, 0.0),
        Point2(15.0, 15.0, 0.0)
    );

and::

    auto line = Line(
        Point2(15.0, 15.0, 0.0),
        Point1(-15.0, -15.0, 0.0)
    );


Instantiation of Geometry
*************************
Each of the geometry types has an instantiation function. These functions are
what we use in the above example code.  Each function takes in a set of
named parameters and then ensures the following:

 #. All required parameters are specified.
 #. No duplicate parameters are specified.
 #. Only parameters that are used are specified.
 #. That the types of the parameters are valid.

Types of the Geometry
*********************

The usage of the the instantiation functions means that the type they return
does not have the same name as the function itself.  These are the type
of each of the geometries used in the examples::

    SphereGeometry sphere = Sphere();
    TriangleGeometry triangle = Triangle(...);
    QuadGeometry quad = Quad(...);
    CylinderGeometry cylinder = Cylinder(...);

    LineGeometry = Line(...);
    MultiLineGeometry multiLine = MultiLine(...);
    PolyLineGeometry<PrimitiveType::LINE_LOOP> polyline
      = PolyLine<PrimitiveType::LINE_LOOP>(...);

    MeshGeometry palmTree = Mesh(...);
    TriangleSoupGeometry jellyGeometry;
    CustomGeometry<PrimitiveType::TRIANGLES> custom;
