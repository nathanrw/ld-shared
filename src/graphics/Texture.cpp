/*****************************************************************************
 * Implementation of Texture class and helpers.
 */

#include <stdexcept>
#include <stbimage/stb_image.h>

#include <graphics/Texture.hpp>

using namespace graphics;
using namespace filesystem;
using namespace Eigen;

/*****************************************************************************/
Texture::Texture(
  GraphicsSystem& tok, 
  TextureTarget bind_to, 
  Path filename
) 
  : GraphicsObject(tok)
{ 
  glGenTextures(1, &m_id); 
  bind(bind_to);

  int n;
  stbi_uc* data = stbi_load(filename.path().c_str(), &m_size[0], &m_size[1], &n, 4);
  if (data == 0) {
    glDeleteTextures(1, &m_id);
    throw std::runtime_error(stbi_failure_reason());
  }

  glTexImage2D(
    get_gl_enum(bind_to), // Target
    0,                    // Level
    GL_RGBA,              // Internal format 
    m_size[0], m_size[1], // W, H
    0,                    // Border (must always == 0)
    GL_RGBA,              // Format
    GL_UNSIGNED_BYTE,     // Data type
    data                  // Data
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

/*****************************************************************************/
void Texture::bind(TextureTarget to)
{ 
  glBindTexture(get_gl_enum(to), m_id); 
}

/*****************************************************************************/
Vector2i Texture::size() const
{
  return m_size;
}

/*****************************************************************************/
Texture::~Texture() 
{ 
  glDeleteTextures(1, &m_id);
}

