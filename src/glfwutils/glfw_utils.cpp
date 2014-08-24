
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <glfwutils/glfw_utils.hpp>

using namespace Eigen;


//----- GLFWToken

// Safely initialise and terminate glfw. Not thread safe.
int GLFWToken::s_instance_count = 0;
  
//*****************************************************************************
GLFWToken::GLFWToken() 
{ 
  if (s_instance_count == 0) {
    if (!glfwInit()) { 
      throw std::runtime_error("Failed to initialise glfw.");
    } else {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
  }
  ++s_instance_count;
}

//*****************************************************************************
GLFWToken::~GLFWToken() 
{ 
  if (--s_instance_count == 0) glfwTerminate(); 
}


//----- GLFWWindow

//*****************************************************************************
std::vector<GLFWEventListener*>& GLFWWindow::get_listeners(GLFWwindow* ptr)
{
  auto ret = reinterpret_cast<GLFWWindow*>(glfwGetWindowUserPointer(ptr));
  assert(ret);

  return ret->m_listeners;
}

//*****************************************************************************
void GLFWWindow::key_callback(
  GLFWwindow* window, 
  int key, 
  int scancode, 
  int action, 
  int mods
)
{
  for (auto listener : get_listeners(window)) {
    switch (action) {
      case GLFW_PRESS:   return listener->on_key_down(key, scancode, mods);
      case GLFW_RELEASE: return listener->on_key_up(key, scancode, mods);
      case GLFW_REPEAT:  return listener->on_key_held(key, scancode, mods);
    }
  }
}

//*****************************************************************************
void GLFWWindow::char_callback(GLFWwindow* window, unsigned int utf_32_char)
{
  for (auto listener : get_listeners(window)) {
    listener->on_char_input(utf_32_char);
  }
}

//*****************************************************************************
void GLFWWindow::mouse_button_callback(
  GLFWwindow* window,
  int button,
  int action,
  int mods
)
{
  for (auto listener : get_listeners(window)) {
    switch (action) {
      case GLFW_PRESS: return listener->on_mouse_button_down(button, mods);
      case GLFW_RELEASE: return listener->on_mouse_button_up(button, mods);
    }
  }
}

//*****************************************************************************
void GLFWWindow::cursor_pos_callback(GLFWwindow* window, double x, double y)
{
  for (auto listener : get_listeners(window)) {
    listener->on_mouse_moved(Vector2d(x, y));
  }
}

//*****************************************************************************
void GLFWWindow::cursor_enter_callback(GLFWwindow* window, int entered)
{
  for (auto listener : get_listeners(window)) {
    if (entered) listener->on_mouse_entered_window();
    else         listener->on_mouse_left_window();
  }
}

//*****************************************************************************
void GLFWWindow::cursor_scroll_callback(GLFWwindow* window, double x, double y)
{
  for (auto listener : get_listeners(window)) {
    listener->on_mouse_scrolled(Vector2d(x, y));
  }
}

//*****************************************************************************
GLFWWindow::GLFWWindow(Vector2i size, std::string title)
  : m_window(0)
{
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  m_window = glfwCreateWindow(
    size[0],
    size[1], 
    title.c_str(), 
    nullptr, 
    nullptr
  );

  if (m_window == nullptr) 
    throw std::runtime_error("Failed to create window.");
  
  glfwSetWindowUserPointer(m_window, this);
  glfwSetKeyCallback(m_window, key_callback);
  glfwSetCharCallback(m_window, char_callback);
  glfwSetMouseButtonCallback(m_window, mouse_button_callback);
  glfwSetCursorPosCallback(m_window, cursor_pos_callback);
  glfwSetCursorEnterCallback(m_window, cursor_enter_callback);
  glfwSetScrollCallback(m_window, cursor_scroll_callback);
}

//*****************************************************************************
void GLFWWindow::make_context_current() 
{ 
  glfwMakeContextCurrent(m_window);
}

//*****************************************************************************
Vector2i GLFWWindow::framebuffer_size() 
{
  Vector2i ret;
  glfwGetFramebufferSize(m_window, &ret[0], &ret[1]);
  return ret;
}

//*****************************************************************************
bool GLFWWindow::should_close() 
{ 
  return !!glfwWindowShouldClose(m_window); 
}

//*****************************************************************************
void GLFWWindow::swap_buffers() 
{ 
  glfwSwapBuffers(m_window); 
}

//*****************************************************************************
void GLFWWindow::add_event_listener(GLFWEventListener& listener)
{
  m_listeners.push_back(&listener);
}

//*****************************************************************************
GLFWWindow::~GLFWWindow() 
{ 
  glfwDestroyWindow(m_window); 
}

//*****************************************************************************
GLFWModifierKeys::GLFWModifierKeys(int mods)
  : m_mods(mods)
{
}

//*****************************************************************************
bool GLFWModifierKeys::shift() const
{
  return !!(m_mods & GLFW_MOD_SHIFT);
}

//*****************************************************************************
bool GLFWModifierKeys::ctrl() const
{
  return !!(m_mods & GLFW_MOD_CONTROL);
}

//*****************************************************************************
bool GLFWModifierKeys::alt() const
{
  return !!(m_mods & GLFW_MOD_ALT);
}

//*****************************************************************************
bool GLFWModifierKeys::super() const
{
  return !!(m_mods & GLFW_MOD_SUPER);
}
