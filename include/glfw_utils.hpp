#pragma once

#include <utility>

class GLFWwindow;

// Safely initialise and terminate glfw. Not thread safe.
class GLFWToken {
public:
  GLFWToken();
  // Initialise glfw if it hasn't been done already. If the operation fails,
  // then throw a runtime_error.

  ~GLFWToken();
  // Destructor. Decrement the instance count and terminate glfw if it
  // hits 0.
private:
  static int s_instance_count;
};

// A std::pair representing a 2D size.
class Size2D : public std::pair<int, int> {
public:
  Size2D() {}
  Size2D(int width, int height) { first = width; second = height; }
  int width() const { return first; }
  int height() const { return second; }
};

// Wraps up a glfw window.
class GLFWWindow {
public:

  GLFWWindow(int width, int height, const char* title);

  void make_context_current();

  Size2D framebuffer_size();
  // Get the framebuffer size.

  bool should_close();
  // Should the window close?

  void swap_buffers();
  // Swap the framebuffers.

  ~GLFWWindow();
  // Destructor.

private:
  GLFWwindow* m_window;
};
