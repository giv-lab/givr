.. _givr-styles:

Styles
==========

.. highlight:: cpp

givr provides the following types of styles.


Lines
--------------------------------------------------------------------------------
The line style is used to render the geometry types that are just lines.
Lines have no normals or shading. They cannot be rendered with phong shading.
Thus the givr::Lines style is necessary.


Parameters
**********
Lines have two parameters. The first is the colour of the line. The second
is the width of the line::

    struct Lines {
        vec3f colour;
        float lineWidth = 1.f;
    };

*NOTE:* Not all opengl implementation allow setting line width to all values.
When you set it to a value it doesn't support, the resulting line width is
usually the closest supported value.

Required Data
*************
Lines require that the geometry you provide it uses one of the following
primitive types:

  - givr::PrimitiveType::LINES
  - givr::PrimitiveType::LINE_LOOP
  - givr::PrimitiveType::LINE_STRIP
  - givr::PrimitiveType::LINES_ADJACENCY
  - givr::PrimitiveType::LINE_STRIP_ADJACENCY

It also requires that the gemoetry provides vertices.

Example
********
::
    //  make a style that renders lines as green and 15 wide.
    auto lineStyle = givr::Lines{vec3f{0.0, 1.0, 0.0}, 15};


Phong
--------------------------------------------------------------------------------
The phong style is a simple style which provides smooth 3D shaded geometry. 

*NOTE*: If you provide normals that are not smooth, then the phong shader
will generate flat shading across the entire triangle. As an example, if you
use the `TriangleSoup` geometry with the phong shader, it will appear as
flat shading as the normals provided by this geometry are not smooth across
adjacent triangles.

Parameters
**********
The phong shader provides a number of parameters::

    struct PhongParameters {
        vec3f colour;
        vec3f lightPosition;
        bool perVertexColour = false;
        float ambientFactor = 0.05f;
        float specularFactor = 0.3f;
        float phongExponent = 8.0f;
    };

The colour is the colour of the entire object. The light position is the
position of the light. If you have provided `colours` as part of your geometry
and would like to use per vertex colours, then you can set `perVertexColour`
to true.  The `ambientFactor`, `specularFactor` and `phongExponent`
variables affect the lighting style.

Required Data
*************
The phong style requires that your geometry uses  one of the following
primitive types:

  - givr::PrimitiveType::TRIANGLES
  - givr::PrimitiveType::TRIANGLE_STRIP
  - givr::PrimitiveType::TRIANGLE_FAN
  - givr::PrimitiveType::TRIANGLES_ADJACENCY
  - givr::PrimitiveType::TRIANGLE_STRIP_ADJACENCY


Example
********
::

    givr::Phong phongStyle;
    phongStyle.colour = vec3f{71.0/255.0, 111.0/255.0, 255.0/255.0};
    phongStyle.lightPosition = vec3f{-60.0, 30.0, -60.0};
