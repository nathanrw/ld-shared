
#include <iostream>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <string>
#include <glfw_utils.hpp>


//----- GLFWToken

// Safely initialise and terminate glfw. Not thread safe.
static int GLFWToken_instance_count = 0;
  
GLFWToken::GLFWToken() { 
  if (GLFWToken_instance_count == 0 && !glfwInit()) 
    throw std::runtime_error("Failed to initialise glfw.");
  ++GLFWToken_instance_count;
}

GLFWToken::~GLFWToken() { 
  if (--GLFWToken_instance_count == 0) glfwTerminate(); 
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
