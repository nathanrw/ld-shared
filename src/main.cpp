/**
 * Main
 *
 **/

#include <iostream>
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include <glfw_utils.hpp>

#include <graphics/Texture.hpp>
#include <graphics/ShaderProgram.hpp>

using namespace graphics;


static void glfw_error_callback(int error, const char* description)
{
  std::cout << "GLFW Error " << error << ": " << description << std::endl;
}

// Main
int main(int argc, char** argv)
{
  glfwSetErrorCallback(glfw_error_callback);
  
  // Initialise glfw
  GLFWToken tok;
  GLFWWindow window(640, 480, "Hello");

  window.make_context_current();

  glewExperimental = GL_TRUE;
  glewInit();

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

  Size2D size = window.framebuffer_size();
  glViewport(0, 0, size.width(), size.height());
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  float points[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
   -0.5f, -0.5f,  0.0f
  };

  GLuint vbo = 0;
  glGenBuffers (1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), points, GL_STATIC_DRAW);

  float texcoords[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  };

  GLuint texcoords_vbo = 0;
  glGenBuffers(1, &texcoords_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
  glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), texcoords, GL_STATIC_DRAW);
  
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  
  std::string vertex_shader =
    "#version 140\n"
    "in vec3 vp;\n"
    "in vec2 vt;\n"
    "out vec2 texture_coordinates;"
    "void main() {\n"
    "  gl_Position = vec4 (vp, 1.0);\n"
    "  texture_coordinates = vt;"
    "}\n";

  std::string fragment_shader =
    "#version 140\n"
    "in vec2 texture_coordinates;"
    "uniform sampler2D texture;"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = texture2D(texture, texture_coordinates);"
    "}";

  VertexShader vs(vertex_shader);
  FragmentShader fs(fragment_shader);

  ShaderProgram shader_program;
  shader_program.attach(vs);
  shader_program.attach(fs);
  shader_program.link();

  Texture lucy(TextureTarget::TEXTURE_2D, "data/textures/lucy.png");

  while (!window.should_close()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lucy.bind(TextureTarget::TEXTURE_2D);
    shader_program.bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    window.swap_buffers();
    glfwPollEvents();
  }

  return 0;
}
