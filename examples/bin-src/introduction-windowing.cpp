#include "givr.h"

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
        // Do something
    });
    exit(EXIT_SUCCESS);
}
