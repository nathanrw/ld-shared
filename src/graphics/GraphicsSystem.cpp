#include <iostream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <graphics/GraphicsSystem.hpp>

using namespace graphics;
using namespace Eigen;

//*****************************************************************************
static void glfw_error_callback(int error, const char* description)
{
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

//*****************************************************************************
GraphicsSystem::GraphicsSystem(Vector2i window_size, std::string window_name)
{
  glfwSetErrorCallback(glfw_error_callback);
  
  // Initialise glfw
  m_glfw_token = new GLFWToken;

  m_window = new GLFWWindow(window_size, window_name);
  m_window->make_context_current();

  //glewExperimental = GL_TRUE;
  glewInit();

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

  Vector2i size = m_window->framebuffer_size();
  glViewport(0, 0, size[0], size[1]);
}

//*****************************************************************************
Vector2i GraphicsSystem::window_size() const
{
  return m_window->framebuffer_size();
}

//*****************************************************************************
GLFWWindow& GraphicsSystem::window()
{
  return *m_window;
}

//*****************************************************************************
GraphicsSystem::~GraphicsSystem()
{
  delete m_window;
  delete m_glfw_token;
}
