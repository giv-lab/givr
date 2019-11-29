Windowing and Input with `givio`
================================
.. highlight:: c++

The first thing your graphics program must do is to create a window and
OpenGL context.  Most libraries which deal with window creation will also
deal with OpenGL context creation in some fashion. They will almost always
deal with user input as well.

The :code:`givio` library does all three for us, and does it with very few
lines of code. The following is a complete sample of using :code:`givio` to
create a window (and OpenGL context), respond to user input (exit when the
escape key is pressed) and then show the window until the program is exited.

.. code-block:: c++
  :linenos:

    #include "io.h"

    int main(void)
    {
        io::GLFWContext windows;
        auto window = windows.create(io::Window::dimensions{640, 480}, "Simple example");
        window.enableVsync(true);

        window.keyboardCommands()
          | io::Key(GLFW_KEY_ESCAPE,
              [&](auto const &/*event*/) { window.shouldClose(); });

        window.run([&](float frameTime) {
            // Do Nothing
        });
        exit(EXIT_SUCCESS);
    }


Lines, 3, 4, 16, and 17 (shown below) are all standard C++ code for
a program. We won't describe them further.


.. code-block:: c++

    int main(void)
    {
        exit(EXIT_SUCCESS);
    }


Line 1 (shown below) includes the :code:`givio` header and makes it available to
us to use.

.. code-block:: c++

    #include "io.h"


Line 5 and 6 (shown below) create the context and then a window which is sized to be
640x480 pixels large and has a title of "Simple example". 

.. code-block:: c++

        io::GLFWContext windows;
        auto window = windows.create(io::Window::dimensions{640, 480}, "Simple example");


Line 7 (shown below) is an optional line which enables a commonly used technique
called v-sync.  This technique ensures that whenever the screen is rendered, it's rendered
just before your monitor updates the image. Doing so avoids the 'tearing' artifact that may
otherwise be present.  However, it does limit your program to run only as fast as your
monitor's refresh rate. 

.. code-block:: c++

        window.enableVsync(true);


Line 9-11 (shown below) shows how you can respond to keyboard input with
:code:`givio`. In this case, we responsd to the escape key by asking the window to close.

.. code-block:: c++

        window.keyboardCommands()
          | io::Key(GLFW_KEY_ESCAPE,
              [&](auto const &/*event*/) { window.shouldClose(); });


Line 13-15 (shown below) is how you ask :code:`givio` to run a main loop.
You might notice that we previously said that :code:`givr` does not provide a main loop.
This is true and intentional so that you can fully control how your program runs.
However, :code:`givio` does provide a main loop as a convenience for those of you
that do not want to control every last aspect.

.. code-block:: c++

        window.run([&](float frameTime) {
            // Do Nothing
        });
        exit(EXIT_SUCCESS);


In summary, :code:`givio` is a library that provides a succinct way to create a window,
OpenGL context and respond to user input. It makes a great partner library for
:code:`givr`.

