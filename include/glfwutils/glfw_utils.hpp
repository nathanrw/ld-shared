#pragma once

#include "Eigen/Dense"

#include <utility>

class GLFWwindow;
class GLFWEventListener;

//*****************************************************************************
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

//*****************************************************************************
// Wraps up a glfw window.
class GLFWWindow {
public:

  GLFWWindow(Eigen::Vector2i size, std::string title);

  void make_context_current();

  Eigen::Vector2i framebuffer_size();
  // Get the framebuffer size.

  bool should_close();
  // Should the window close?

  void swap_buffers();
  // Swap the framebuffers.
  
  void add_event_listener(GLFWEventListener& listener);
  // Events will be added to the given queue.

  ~GLFWWindow();
  // Destructor.

private:
  
  static std::vector<GLFWEventListener*>& get_listeners(GLFWwindow* window);
  static void key_callback(GLFWwindow* window, int, int, int, int);
  static void mouse_button_callback(GLFWwindow* window, int, int, int);
  static void cursor_pos_callback(GLFWwindow* window, double, double);
  static void cursor_enter_callback(GLFWwindow* window, int);
  static void cursor_scroll_callback(GLFWwindow* window, double, double);
  static void char_callback(GLFWwindow* window, unsigned int);

  GLFWwindow* m_window;
  std::vector<GLFWEventListener*> m_listeners;
};

//*****************************************************************************
// Wraps up a modifier keys bit field.
class GLFWModifierKeys {
public:
  GLFWModifierKeys(int mods);
  bool shift() const;
  bool ctrl() const;
  bool alt() const;
  bool super() const;
private:
  int m_mods;
};

//*****************************************************************************
// Provides a bevy of optional event handling methods. Derived classes can
// override the ones they are interested in.
class GLFWEventListener {
public:

  // Mouse events
  virtual void on_mouse_moved(Eigen::Vector2d new_mouse_position) {};
  virtual void on_mouse_entered_window() {};
  virtual void on_mouse_left_window() {};
  virtual void on_mouse_scrolled(Eigen::Vector2d scroll_offset) {};
  virtual void on_mouse_button_up(int button, GLFWModifierKeys mods) {};
  virtual void on_mouse_button_down(int button, GLFWModifierKeys mods) {};

  // Keyboard events
  virtual void on_key_up(int key, int scancode, GLFWModifierKeys mods) {};
  virtual void on_key_down(int key, int scancode, GLFWModifierKeys mods) {};
  virtual void on_key_held(int key, int scancode, GLFWModifierKeys mods) {};

  // Text entry events.
  virtual void on_char_input(unsigned int utf_32_char) {};
};
