/**
 * Classes for dealing with shader programs.
 *
 **/

#pragma once

#include <GL/glew.h>
#include <string>

#include <graphics/GraphicsObject.hpp>

#include <filesystem/Path.hpp>

namespace graphics {

  class Shader;

  /**
   * Class for initialising and managing an OpenGL shader program object.
   **/
  class ShaderProgram : public GraphicsObject {
  public:
    explicit ShaderProgram(GraphicsToken& tok);
    void attach(const Shader& shader);
    bool link();
    void bind();
    ~ShaderProgram();
  private:
    GLuint m_id;
  };
  
  /**
   * Base class for initialising and managing an OpenGL shader object. You don't
   * need to use this class directly - use VertexShader or FragmentShader.
   **/
  class Shader : public GraphicsObject {
  public:
  
    // No public methods - see VertexShader and FragmentShader below.
  
  protected:
  
    /**
     * Constructor. Creates an OpenGL shader object of the appropriate type,
     * uploads the given source code and attempts to compile it. Note that
     * "source" expects the actual shader source code, not a filename!
     *
     * Throws std::runtime_error if compilation fails for whatever reason.
     **/
    Shader(GraphicsToken& tok, GLenum type, std::string source);
    
    /**
     * As above but taking a filename.
     **/
    Shader(GraphicsToken& tok, GLenum type, filesystem::Path filename);
  
    /**
     * Dtor. Destroys the underlying OpenGL object.
     **/
    virtual ~Shader();
  
  private:
    void initialise(GLenum type, std::string source);
    bool invalid();
    std::string info_log();
    friend class ShaderProgram;
    GLuint m_id;
  };
  
  /**
   * Class for initialising and managing an OpenGL fragment shader object.
   **/
  class FragmentShader : public Shader {
  public:
    FragmentShader(GraphicsToken& tok, std::string source);
    FragmentShader(GraphicsToken& tok, filesystem::Path path);
  };
  
  /**
   * Class for initialising and managing an OpenGL vertex shader object.
   **/
  class VertexShader : public Shader {
  public:
    VertexShader(GraphicsToken& tok, std::string source);
    VertexShader(GraphicsToken& tok, filesystem::Path path);
  };
  
}
