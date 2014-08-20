/**
 * Main
 *
 */

#include <iostream>
#include <stdexcept>
#include <string>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include <filesystem/Path.hpp>

#include <graphics/GraphicsSystem.hpp>
#include <graphics/Texture.hpp>
#include <graphics/ShaderProgram.hpp>
#include <graphics/BufferObjects.hpp>

using namespace graphics;
using namespace filesystem;
using namespace Eigen;

// Main
int main(int argc, char** argv)
{
  GraphicsSystem graphics(Size2D(800, 600), "Hello");
  GraphicsToken& gtok = graphics.token();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  // Construct a vertex array to hold our rendering state.
  VertexArrayObject vao(gtok);

  // Add a buffer containing vertex positions;
  Vector3f points[] = {
    Vector3f( 0.0f,  0.5f,  0.0f),
    Vector3f( 0.5f, -0.5f,  0.0f),
    Vector3f(-0.5f, -0.5f,  0.0f)
  };

  VertexBufferObject points_vbo(
    gtok,
    BufferTarget::ARRAY_BUFFER, 
    BufferUsage::STATIC_DRAW
  );
  points_vbo.fill(sizeof(points), points);

  AttributeIndex pos_attribute(0);
  vao.enable_attribute(pos_attribute);
  vao.attribute_pointer(
    pos_attribute, 
    points_vbo, 
    ComponentCount::THREE, 
    DataType::FLOAT,
    false,
    0
  );

  // Add a buffer containing texture coordinates.
  Vector2f texcoords[] = {
    Vector2f(0.0f, 0.0f),
    Vector2f(0.0f, 1.0f),
    Vector2f(1.0f, 1.0f)
  };
  
  VertexBufferObject texcoords_vbo(
    gtok,
    BufferTarget::ARRAY_BUFFER, 
    BufferUsage::STATIC_DRAW
  );
  texcoords_vbo.fill(sizeof(texcoords), texcoords);
  
  AttributeIndex texture_attribute(1);
  vao.enable_attribute(texture_attribute);
  vao.attribute_pointer(
    texture_attribute,
    texcoords_vbo,
    ComponentCount::TWO,
    DataType::FLOAT,
    false,
    0
  );

  VertexShader vs(gtok, Path("data/shaders/basic.glsl.v"));
  FragmentShader fs(gtok, Path("data/shaders/basic.glsl.f"));
  
  ShaderProgram shader_program(gtok);
  shader_program.attach(vs);
  shader_program.attach(fs);
  shader_program.link();

  Texture lucy(
    gtok, 
    TextureTarget::TEXTURE_2D, 
    Path("data/textures/lucy.png")
  );

  while (!graphics.window().should_close()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lucy.bind(TextureTarget::TEXTURE_2D);
    shader_program.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    graphics.window().swap_buffers();
    glfwPollEvents();
  }

  return 0;
}
