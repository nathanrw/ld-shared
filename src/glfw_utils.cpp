
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <glfw_utils.hpp>


//----- GLFWToken

// Safely initialise and terminate glfw. Not thread safe.
int GLFWToken::s_instance_count = 0;
  
GLFWToken::GLFWToken() { 
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

GLFWToken::~GLFWToken() { 
  if (--s_instance_count == 0) glfwTerminate(); 
}


//----- GLFWWindow

GLFWWindow::GLFWWindow(int width, int height, const char* title)
  : m_window(glfwCreateWindow(width, height, title, nullptr, nullptr))
{
  if (m_window == nullptr) 
    throw std::runtime_error("Failed to create window.");
}

void GLFWWindow::make_context_current() { glfwMakeContextCurrent(m_window); }

Size2D GLFWWindow::framebuffer_size() {
  Size2D ret;
  glfwGetFramebufferSize(m_window, &ret.first, &ret.second);
  return ret;
}

bool GLFWWindow::should_close() { return !!glfwWindowShouldClose(m_window); }
void GLFWWindow::swap_buffers() { glfwSwapBuffers(m_window); }
GLFWWindow::~GLFWWindow() { glfwDestroyWindow(m_window); }
