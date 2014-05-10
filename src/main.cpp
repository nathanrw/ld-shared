/**
 * Main
 *
 **/

#include <iostream>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <string>
#include <glfw_utils.hpp>


static void glfw_error_callback(int error, const char* description)
{
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

// Main
int main(int argc, char** argv)
{
  glfwSetErrorCallback(glfw_error_callback);

  GLFWToken tok;
  GLFWWindow window(640, 480, "Hello");

  window.make_context_current();

  Size2D size = window.framebuffer_size();
  glViewport(0, 0, size.width(), size.height());

  while (!window.should_close()) {
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window.swap_buffers();
    glfwPollEvents();
  }

  return 0;
}
