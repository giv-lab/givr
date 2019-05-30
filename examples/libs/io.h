#pragma once
#include <GLFW/glfw3.h>

// io
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


namespace io {

template <typename Index, typename... Args> class Commands {
public:
  using index_t = Index;
  using function_t = std::function<void(Args &&...)>;
  using map = std::unordered_map<index_t, function_t>;
  using command_type = typename map::value_type;

  void execute(index_t index, Args &&... args) {
    auto command = m_commands.find(index);
    if (command != m_commands.end())
      command->second(std::forward<Args>(args)...);
  }

  void bind(index_t index, function_t &&func) {
    m_commands[index] = std::forward<function_t>(func);
  }
  void bind(command_type &&command) {
    m_commands.insert(std::forward<command_type>(command));
  }

  Commands &operator|(command_type &&command) {
    bind(std::forward<command_type>(command));
    return *this;
  }

  static command_type makeCommand(index_t index, function_t &&function) {
    return {index, std::move(function)};
  }

private:
  map m_commands;
};

struct KeyboardEvent {
  int key;
  int action;
  int mods;
};

using KeyboardCommands = Commands<int, KeyboardEvent>;
using Key = KeyboardCommands::command_type;

struct MouseEvent {
  int button;
  int action;
  int mods;
};

using MouseCommands = Commands<int, MouseEvent>;
using MouseButton = MouseCommands::command_type;

struct CursorPosition {
  double x;
  double y;
};
using CursorCommand = std::function<void(CursorPosition event)>;

class Window {
public: // types
  struct dimensions {
    uint32_t width;
    uint32_t height;
  };

  class Properties {
  public: // functions
    Properties(dimensions dimensions) : m_dimensions(dimensions) {}
    Properties(dimensions dimensions, std::string title)
        : m_dimensions(dimensions), m_title(title) {}

    dimensions size() const { return m_dimensions; }
    uint32_t width() const { return m_dimensions.width; }
    uint32_t height() const { return m_dimensions.height; }
    std::string title() const { return m_title; }

    void title(std::string title) { m_title = title; }
    void size(dimensions dimensions) { m_dimensions = dimensions; }
    void width(uint32_t width) { m_dimensions.width = width; }
    void height(uint32_t height) { m_dimensions.height = height; }

  private: // variables
    dimensions m_dimensions;
    std::string m_title;
  };

  using destroyFunction = void (*)(GLFWwindow *);
  using handle_t = std::unique_ptr<GLFWwindow, destroyFunction>;
  using cursor_function = std::function<void (*)(CursorPosition event)>;

  template <typename... PropertiesArgs>
  Window(PropertiesArgs &&... properties)
      : m_properties(std::forward<PropertiesArgs...>(properties...)),
        m_handle(nullptr, destroy) {
    init(true);
  }

  ~Window() = default;
  Window(Window const &o) = delete;
  Window& operator=(Window const &o) = delete;

  Window(Window &&o) noexcept
      : m_properties(std::move(o.m_properties))
      , m_handle(std::move(o.m_handle))
      , m_frameBufferSize(std::move(o.m_frameBufferSize))
      , m_keyboardCommands(std::move(o.m_keyboardCommands))
      , m_mouseCommands(std::move(o.m_mouseCommands))
      , m_cursorCommand(std::move(o.m_cursorCommand))
  {
      init(false);
  }
  Window& operator=(Window&& o) {
      m_properties = std::move(o.m_properties);
      m_handle = std::move(o.m_handle);
      m_frameBufferSize = std::move(o.m_frameBufferSize);
      m_keyboardCommands = std::move(o.m_keyboardCommands);
      m_mouseCommands = std::move(o.m_mouseCommands);
      m_cursorCommand = std::move(o.m_cursorCommand);
      init(false);
      return *this;
  }


public: // functions
  void run(std::function<void(float)> const &render) {
    double previous_time = glfwGetTime();
    while (!glfwWindowShouldClose(handle())) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      double current_time = glfwGetTime();
      double delta_t = current_time - previous_time;
      previous_time = current_time;
      render((float)delta_t);
      glfwPollEvents();
      glfwSwapBuffers(handle());
    }
  }
  void enableVsync(bool use) {
    if (use)
      glfwSwapInterval(1); // frames between
    else
      glfwSwapInterval(0); // 0 frames between
  }



  void shouldClose() { glfwSetWindowShouldClose(handle(), GLFW_TRUE); }

  dimensions windowSize() const { return m_properties.size(); }

  dimensions framebufferSize() const { return m_frameBufferSize; }

  void setCallbacks() {

    // Window size
    glfwSetWindowSizeCallback(
        handle(), [](GLFWwindow *window, int width, int height) {
          auto *windowData =
              static_cast<Window *>(glfwGetWindowUserPointer(window));
          windowData->m_properties.width(width);
          windowData->m_properties.height(height);
        });

    glfwSetFramebufferSizeCallback(
        handle(), [](GLFWwindow *window, int width, int height) {
          auto *windowData =
              static_cast<Window *>(glfwGetWindowUserPointer(window));
          windowData->m_frameBufferSize = {(uint32_t)width, (uint32_t)height};
          glViewport(0, 0, width, height);
        });

    // Keyboard
    glfwSetKeyCallback(handle(), [](GLFWwindow *window, int key, int /*scancode*/,
                                    int action, int mods) {
      auto *windowData =
          static_cast<Window *>(glfwGetWindowUserPointer(window));
      KeyboardEvent event = {key, action, mods};
      windowData->m_keyboardCommands.execute(key, std::move(event));
    });

    // Mouse
    glfwSetMouseButtonCallback(
        handle(), [](GLFWwindow *window, int button, int action, int mods) {
          auto *windowData =
              static_cast<Window *>(glfwGetWindowUserPointer(window));

          MouseEvent event = {button, action, mods};
          windowData->m_mouseCommands.execute(button, std::move(event));
        });

    glfwSetCursorPosCallback(
        handle(), [](GLFWwindow *window, double x, double y) {
          auto *windowData =
              static_cast<Window *>(glfwGetWindowUserPointer(window));
          CursorPosition event = {x, y};
          windowData->m_cursorCommand(event);
        });
  }

  KeyboardCommands &keyboardCommands() { return m_keyboardCommands; }
  MouseCommands &mouseCommands() { return m_mouseCommands; }
  CursorCommand &cursorCommand() { return m_cursorCommand; }

  int width() const { return m_properties.width(); }
  int height() const { return m_properties.height(); }
  std::string title() const { return m_properties.title(); }
  dimensions size() const { return m_properties.size(); }
  dimensions frameBufferSize() const { return m_frameBufferSize; }

private: // functions
  friend class GLFWContext;

  GLFWwindow *create(Properties const &properties) {
    return glfwCreateWindow(properties.width(),         //
                            properties.height(),        //
                            properties.title().c_str(), //
                            nullptr,                    //
                            nullptr);
  }

  GLFWwindow *handle() const { return m_handle.get(); }

  static void destroy(GLFWwindow *window) { glfwDestroyWindow(window); }

  void init(bool createWindow) {
    if (createWindow) m_handle.reset(create(m_properties));
    glfwSetWindowUserPointer(handle(), this);
    setCallbacks();
  }

public: // variables
  Properties m_properties;
  handle_t m_handle;
  dimensions m_frameBufferSize;
  KeyboardCommands m_keyboardCommands;
  MouseCommands m_mouseCommands;
  CursorCommand m_cursorCommand = [](CursorPosition /*event*/) {};
}; // namespace io

class GLFWContext {
public: // funcitons
  GLFWContext() {
    if (!initalize())
      throw;
    setup();
  }

  ~GLFWContext() { shutdown(); }

  bool isInitialized() const {
    return m_isGLFWInitialized == GLFW_TRUE; // 1
  }

  template <typename... PropertiesArgs>
  Window create(PropertiesArgs &&... properties) {
    if (!isInitialized())
      throw;

    auto w = Window(Window::Properties(properties...));
    makeCurrent(w);
    return w;
  }

  void makeCurrent(Window const &window) {
    glfwMakeContextCurrent(window.handle());
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  }

private: // functions
  bool initalize() {
    m_isGLFWInitialized = glfwInit();
    return isInitialized();
  }

  void setup() {
    int glfwMajor, glfwMinor, glfwRevision;
    glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
    std::cout << "GLFW Version: " << glfwMajor << '.' << glfwMinor << '.'
              << glfwRevision << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  }

  void shutdown() { glfwTerminate(); }

private:                                    // variables
  int32_t m_isGLFWInitialized = GLFW_FALSE; // 0
};

} // namespace io
