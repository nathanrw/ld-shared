//*****************************************************************************
// Classes for dealing with OpenGL textures.
//
// e.g.
//
//   Texture texture(TextureTarget::TEXTURE_2D, "data/textures/lucy.png");

#pragma once

#include <GL/glew.h>
#include <string>

#include <filesystem/Path.hpp>

#include <graphics/GraphicsObject.hpp>

#include <Eigen/Dense>

namespace graphics {
  
  //***************************************************************************
  // Type safe alternative to GLenum.
  enum class TextureTarget {
    TEXTURE_1D = GL_TEXTURE_1D,
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_3D = GL_TEXTURE_3D,
    TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
    TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
    TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
    TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
    TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
    TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
    TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
  };

  inline GLenum get_gl_enum(TextureTarget t) { return static_cast<GLenum>(t); }
  // Convert a TextureTarget to a GLenum for passing to OpenGL functions.

  //***************************************************************************
  // Class for initialising and managing an OpenGL texture. Knows how to read
  // texture data from files.
  class Texture : public GraphicsObject {
  public:
    
    Texture(
      GraphicsSystem& tok, 
      TextureTarget bind_to, 
      filesystem::Path filename
    );
    // Construct an OpenGL texture object and read data from the given file
    // into it. Takes a texture target because OpenGL is mad balls and requires
    // one for loading data into a texture.
    // 
    // If anything bad happens, a std::runtime_error is thrown.
    //
    // There are probably situations where feeding garbage to this function 
    // will just cause OpenGL to do something arbitrary - so don't.
    
    void bind(TextureTarget to);
    // Bind this texture to the given target.
    
    Eigen::Vector2i size() const;
    // Get the size of the texture.
    
    ~Texture();
    // Dtor. Frees the underlying OpenGL texture.

  private:
    GLuint m_id;
    Eigen::Vector2i m_size;
  };
}
