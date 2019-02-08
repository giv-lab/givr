.. _givr-geometry:

Geometry
==========

.. highlight:: cpp

givr provides the following types of geometry:


Triangle
--------------------------------------------------------------------------------

The Triangle geometry is used to create a single triangle.

Parameters
**********
It has 3 parameters, one for each point of the triangle::

    struct Triangle {
        vec3f p1;
        vec3f p2;
        vec3f p3;
    }

Data
******
The triangle geometry these pieces of data which are made available to the style:
  * vertices 
  * normals

Example
********
::

   vec3f p1 = ...;
   vec3f p2 = ...;
   vec3f p3 = ...;
   givr::Triangle t{p1, p2, p3};




Line
--------------------------------------------------------------------------------

The line geometry is for generating a single line segment.

Parameters
************
It has two parameters, one for each point on either side of the line segment::

   struct line {
      vec3f p1;
      vec3f p2;
   };

Data
******
The line geometry provides this data to the style:
  * vertices 

Example
*******
::

   givr::line l;
   l.p1 = vec3f{...};
   l.p2 = vec3f{...};


MultiLine
--------------------------------------------------------------------------------

A MultiLine is simply a series of line segments which may or may not
be connected. It is analogous to the GL_LINES rendering type.

Parameters
************
It takes a list of lines as its parameters::

    struct MultiLine {
        std::vector<line> segments;
    }

You can also add line segments using the following API::

    MultiLine l;
    l.addLine(givr::line{p1, p2});
    l.addLine(p1, p2);

Data
******
The MultiLine geometry provides this data to the style:
  * vertices 

Example
*******
::

   givr::MultiLine ml;
   ml.segments.push_back(givr::line{p1, p2});
   ml.segments.push_back(givr::line{p3, p4});


Polyline
--------------------------------------------------------------------------------

A polyline is composed of a series of points where each line segment connects
the current point with the previous point. It is valid for any number of
points greater than 1.

Parameters
************
It is a templated class, which takes givr::PrimitiveType as the template
parameter. This template parameter may be set to one of two values:

  * `givr::PrimitiveType::LINE_LOOP`
  * `givr::PrimitiveType::LINE_STRIP`

If you use `LINE_LOOP`, the final point will be connected by a line segment with
the first point. If you use `LINE_STRIP` then it will not be. This parameter
is a template parameter so that we can do compile time checking to ensure it is
set to the right value. 

The class takes a list of points as parameters::

    template <PrimitiveType LineType>
    struct Polyline {
        std::vector<vec3f> vertices;
    }

Data
******
The PolyLine geometry provides this data to the style:
  * vertices 

Example
*******
::

   givr::Polyline<givr::PrimitiveType::LINE_LOOP> pl;
   pl.vertices.push_back(vec3f{...});
   pl.vertices.push_back(vec3f{...});
   pl.vertices.push_back(vec3f{...});


Sphere
--------------------------------------------------------------------------------

The sphere geometry is used to generate a set of triangles which approximate
a sphere.  By default the sphere is a unit sphere, centred around the
origin. In order to change its radius and position, you will need to use
the model matrix provided to the draw call or the or to the addInstance call.


Parameters
************
It has two parameters, which control how many triangles are generated to approximate
the sphere. They are called, `azimuthPoints` and `altitudePoints` respectively.
They are used to control how often we sample the sphere along the latitude and longitude
in order to generate triangles. They are set to 20 by default, and can be
increased if you want a higher quality sphere::

    struct Sphere {
        std::size_t azimuthPoints = 20;
        std::size_t altitudePoints = 20;
    }

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

   givr::Sphere s;
   auto ctx = createRenderable(s, phongStyle);
   ...
   givr::mat4f m{1.f}; // identity matrix
   m = scale(m, vec3f{2.f, 2.f, 2.f});
   givr::draw(s, view, m);



Cylinder
--------------------------------------------------------------------------------
The Cylinder geometry allows you to place a cylinder that connects two points.
It's often used in place of GL_LINES as it is actually a 3D object, while GL_LINES
are not. The current implementation is an open-faced cylinder.

Parameters
************
It requires two parameters, the end points. It has two additional parameters
which control its radius and the number of times to sample when generating
the triangles::

    struct Cylinder {
        vec3f p1;
        vec3f p2;
        float radius = 1.0f;
        std::size_t points = 20;
    }

Data
******
It generates this data for the style to use:
   - vertices
   - normals
   - indices


Example
*******
::
   givr::Cylinder cyl = givr::Cylinder{p1, p2};


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
you move the program between machines::

    struct Mesh {
        std::string filename;
    }

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

    givr::Mesh palm_tree{"./models/Palm_Tree.obj"};

Triangle Soup
--------------------------------------------------------------------------------
This is the first option for defining your own custom geometry. It's slightly
easier to use, but also slightly less efficient.

Triangle soup is an affectionate name for when you have a bunch of triangles
representing an object, but no implicit connectivity or topology. This
geometry type is like the custom geometry in that it allows you to easily
build new shapes surfaces or other items, but it provides a slightly easier
to use interface to do so.

*NOTE*: This type of geometry produces normals for each triangle, and assigns
that normal to each vertex of that triangle.  In addition, each vertex of the
triangle is explicitly represented in the vertices array regardless of whether
other triangles share the same vertex. The result of this is that they shading
will not be smooth across the edges of triangles. If you want custom geometry
with smooth shading, you will need to use givr::CustomGeometry (see below).

Parameters
************
It has a single parameter which is a vector of triangles::

    struct TriangleSoup {
        std::vector<Triangle> triangles;
    }

Data
******

The triangle geometry these pieces of data which are made available to the style:
  * vertices 
  * normals

Example
********
::

   Triangle t1 = ...;
   Triangle t2 = ...;
   Triangle t3 = ...;
   givr::TriangleSoup ts;
   ts.triangles.push_back(t1);
   ts.triangles.push_back(t2);
   ts.triangles.push_back(t3);


Or more likely you will loop over the elements in your animation/simulation
and turn them into a series of triangles::

   givr::TriangleSoup ts;
   // Loop over all objects in your simulation/animation
   for(int i = 0; i < my_simulation.objects.size(); ++i) {

      // Get a reference to the object
      object const &o = my_simulation.objects[i];

      // Turn that object into a Triangle (or triangles!)
      TriangleSoup t{
         o.get_point1(),
         o.get_point2(),
         o.get_point3()
      };

      // Add that triangle to the triangle soup
      ts.triangles.push_back(t);
   }

As a specific example, here is how I generated the triangles for the sides
of my jelly cube for the mass springs assignment.  I stored my particle masses
in a 1D vector, and then I painstakingly did all of the index math to generate
triangles. It wasn't fun, I'm sure there are better ways::

   givr::TriangleSoup jelly_geometry;

   void update_jelly_geometry() {
       // This gets called for every frame, so it's not hyper efficient, but
       // reasonable for 60ish fps
       jelly_geometry.triangles.clear();

       auto pos = [&](std::size_t i, std::size_t j, std::size_t k) {
           return jelly.particles[(i*(resolution*resolution)) + (j*resolution) + k].position;
       };
       auto add_triangle = [&](vec3f const &p1, vec3f const &p2, vec3f const &p3) {
           jelly_geometry.triangles.push_back(givr::Triangle{p1, p2, p3});
       };

       for (std::size_t i = 0; i < resolution; ++i) {
           for (std::size_t j = 0; j < resolution; ++j) {
               for (std::size_t k = 0; k < resolution; ++k) {
                   if (i == 0  && j!=0 && k!=0) {
                       add_triangle(pos(i, j-1, k-1), pos(i, j, k), pos(i, j, k-1));
                       add_triangle(pos(i, j-1, k-1), pos(i, j-1, k), pos(i, j, k));
                   }
                   if (i +1 == resolution  && j +1 != resolution && k != 0) {
                       add_triangle(pos(i, j+1, k-1), pos(i, j, k), pos(i, j, k-1));
                       add_triangle(pos(i, j+1, k-1), pos(i, j+1, k), pos(i, j, k));
                   }
                   if (j == 0  && i!=0 && k!=0) {
                       add_triangle(pos(i-1, j, k-1), pos(i, j, k), pos(i, j, k-1));
                       add_triangle(pos(i-1, j, k-1), pos(i-1, j, k), pos(i, j, k));
                   }
                   if (j +1 == resolution  && i +1 != resolution && k != 0) {
                       add_triangle(pos(i+1, j, k-1), pos(i, j, k), pos(i, j, k-1));
                       add_triangle(pos(i+1, j, k-1), pos(i+1, j, k), pos(i, j, k));
                   }
                   if (k == 0  && i!=0 && j!=0) {
                       add_triangle(pos(i-1, j-1, k), pos(i, j, k), pos(i, j-1, k));
                       add_triangle(pos(i-1, j-1, k), pos(i-1, j, k), pos(i, j, k));
                   }
                   if (k +1 == resolution  && i +1 != resolution && j != 0) {
                       add_triangle(pos(i+1, j-1, k), pos(i, j, k), pos(i, j-1, k));
                       add_triangle(pos(i+1, j-1, k), pos(i+1, j, k), pos(i, j, k));
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
checking. As a result, you are responsible for this particular renderer. 

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

*NOTE*: None of the current styles use 

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
of geometry, then you should be willing to read any o f the already existing
tutorials on how to setup these sorts of buffers for rendering. The exact format
depends on whether it's indexed, which primitive type you are using etc.
