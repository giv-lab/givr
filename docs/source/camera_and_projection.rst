.. _givr-camera-and-projection:

Camera and Projection
======================

The camera and projection system within givr is still undergoing quite
work to design it to be more friendly and easy to use. As with all of
givr, feedback is welcome.

There are three components involved in this system, the view context,
the camera and the projection.


View Context
--------------------------------------------------------------------------------
The view context is provided by givr and is a simple templated struct which holds
a reference to a camera and projection class.

In the example code below and in the examples provided throughout the documentation
the type for the camera is `givr::TurnTable` and the type for the projection is
`givr::PerspectiveView`.  The instance of the camera is available via the 
`view.camera` attribute and the instance of the projection is available via the
`view.projection` attribute.  Both types must be able to be instantiated with
no parameters. 

Example
********
::

    givr::ViewContext<givr::TurnTable, givr::PerspectiveView> view;



Camera
--------------------------------------------------------------------------------
The only camera type built into givr at the moment is a TurnTable camera,
which provides some simple TurnTable type controls. You can rotate around
two axes and zoom in and out.

If you would like to provide your own camera, you may do so. In order to do
so you must do four things:

1. Create a custom type (struct/class) that represents your camera.
2. Define a `mat4f getViewMatrix(MyCameraT const &camera)` function which
   within that class generates the view matrix from the camera instance.
3. Define a `mat4f getViewPosition(MyCameraT const &camera)` function which
   within that class generates the view position for given camera instance.
4. Specify this type of camera for your view context

The actual struct/class that you define may have as many attributes and
methods as needed. None of them will be used by givr directly. Whenever the
style needs the view position and view matrix, it will call the appropriate
method defined above.

Example
*******
::

   struct MyCamera {
       static mat4f getViewMatrix(MyCamera const &c) {
           auto up = ...;
           auto pos =  ...;
           auto lookAtPoint =  ...;
           return lookAt(pos, up, lookAtPoint);
       }

       static vec3f getViewPosition(MyCamera const &c) {
           auto pos = ...;
           return pos;
       }
   };
   ...
   givr::ViewContext<MyCamera, givr::PerspectiveView> view;

Projection
--------------------------------------------------------------------------------
Similar to the Camera class above, the projection class can also be customized.
you need do 3 things for the project class

1. Create a custom type (struct/class) that represents your projection.
2. Define a `mat4f getProjectionMatrix(MyProjectionT const &camera)`
   within that class function which generates the view matrix from the
   projection instance.
3. Specify this type of projection for your view context
