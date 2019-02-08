Quickstart
==========
.. highlight:: c++

Here is very simple program that renders a sphere going in a circle.
I have removed all of the windowing/opengl setup code so you can focus
on the basic API of how to draw something::

   #include <givr.h>

   ...

   /* Instantiate camera/projection objects */
   givr::ViewContext<givr::TurnTable, givr::PerspectiveView> view;

   /* Create Geometry Style and rendering context */
   givr::Phong phongStyle;
   phongStyle.colour = vec3f{1.0, 1.0, 0.1529};
   phongStyle.lightPosition = vec3f{2.0, 2.0, 15.0};
   auto sphere = createRenderable(givr::Sphere{}, phongStyle);

   ...

   /* Setup the model matrix to translate it to the appropriate spot */
   float u = 0.f;
   loop {
      x = 10.0 * cos(u);
      y = 10.0 * sin(u);
      u += 0.05;
      mat4f m = glm::translate(mat4f{1.f}, vec3f{x, y, 0.});

      /* Draw it in that position */
      draw(sphere, view, m);
   }
