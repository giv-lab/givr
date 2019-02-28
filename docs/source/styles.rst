.. _givr-styles:
.. role:: cpp(code)
   :language: cpp

Styles
==========

.. highlight:: cpp

givr provides the following types of styles.


GL Lines
--------------------------------------------------------------------------------
The line style is used to render the geometry types that are rendered using
OpenGL lines.  Lines have no normals or shading. They cannot be rendered with
phong shading.  Thus the :cpp:`givr::styles::GL_Line` style is necessary.


Parameters
**********

Lines have two parameters:

 - :cpp:`Colour(float, float, float)`: The colour of the line.

   - **Required**

 - :cpp:`Width(float)`: The width of the line.

   - *Default*: :cpp:`1.f`


*NOTE:* Not all opengl implementations allow setting line width to all values.
When you set it to a value it doesn't support, the resulting line width is
usually the closest supported value. Some implementations only support line
width of 1.0. If you need better control over your lines, then consider the
cylinder class

Required Data
*************
Lines require that the geometry you provide it uses one of the following
primitive types:

  - :cpp:`givr::PrimitiveType::LINES`
  - :cpp:`givr::PrimitiveType::LINE_LOOP`
  - :cpp:`givr::PrimitiveType::LINE_STRIP`
  - :cpp:`givr::PrimitiveType::LINES_ADJACENCY`
  - :cpp:`givr::PrimitiveType::LINE_STRIP_ADJACENCY`

It also requires that the geometry provides vertices.

Example
********

A simple example::

    //  make a style that renders lines as green and 15 wide.
    auto lineStyle = GL_Line(Width(15.), Colour(0.0, 1.0, 0.0));

NoShading
--------------------------------------------------------------------------------
The NoShading style is a simple style which simply fills in geometry with
a single colour. No shading is done. Useful for things like using cylinders
to approximate lines in an orthographic view.

Parameters
**********
The NoShading style provides a single parameter

 - :cpp:`Colour(float, float, float)`: The colour of the object 

   - **Required**


Required Data
*************
It has no restrictions on the type of geometry it can render.

Example
********

A simple example::

    auto noshading = NoShading(Colour(1.0, 0.0, 0.0));


Phong
--------------------------------------------------------------------------------
The phong style is a simple style which provides 3D shaded geometry. 

*NOTE*: If you provide normals that are not smooth, then the phong shader
will generate flat shading. As an example, if you use the `TriangleSoup` geometry
with the phong shader, it will use flat shading as the normals provided by this
geometry are not smooth across adjacent triangles edges. 

Parameters
**********
The phong shader provides a number of parameters

 - :cpp:`Colour(float, float, float)`: The colour of the object 

   - **Required**

 - :cpp:`LightPosition(float, float, float)`: The position of the light.

   - **Required**

 - :cpp:`AmbientFactor(float)`: The Ambient lighting factor.

   - *Default*: :cpp:`0.05f`

 - :cpp:`SpecularFactor(float)`: The Specular lighting factor.

   - *Default*: :cpp:`0.3f`

 - :cpp:`PhongExponent(float)`: The Phong Exponent.

   - *Default*: :cpp:`0.8f`

 - :cpp:`ShowWireFrame(bool)`: Whether to show wireframe or not.  Uses the geometry shader.

   - *Default*: :cpp:`false`

 - :cpp:`WireFrameColour(float, float, float)`: The colour of the wireframe.

   - *Default*: :cpp:`0.f, 0.f, 0.f`

 - :cpp:`WireFrameWidth(float)`: The approximate width of the wireframe lines.
   
   - *Default* :cpp:`1.5f`

 - :cpp:`GenerateNormals(bool)`: Whether to automatically generate normals for
   each triangle. Uses the geometry shader. Normals are per-triangle and as such
   produce flat shading.
   
   - *Default*: :cpp:`false`

 - :cpp:`PerVertexColour(bool)`: Whether to use the colours specified as part
   of the geometry, where each vertex has its own colour value.
   
   - *Default*: :cpp:`false`


Required Data
*************
The phong style requires that your geometry uses  one of the following
primitive types:

  - :cpp:`givr::PrimitiveType::TRIANGLES`
  - :cpp:`givr::PrimitiveType::TRIANGLE_STRIP`
  - :cpp:`givr::PrimitiveType::TRIANGLE_FAN`
  - :cpp:`givr::PrimitiveType::TRIANGLES_ADJACENCY`
  - :cpp:`givr::PrimitiveType::TRIANGLE_STRIP_ADJACENCY`

It also requires that the geometry provides vertices. 

Example
********

A simple example::

    auto phongStyle = Phong(
        LightPosition(0.0, 0.0, 100.0),
        Colour(1.0, 1.0, 0.1529)
    );
