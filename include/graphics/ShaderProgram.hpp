/**
 * Classes for dealing with shader programs.
 *
 * E.g.
 *
 *   VertexShader vs(vertex_shader_source);
 *   FragmentShader fs(fragment_shader_source);
 *   ShaderProgram program;
 *   program.attach(vs);
 *   program.attach(fs);
 *   program.link();
 *
 **/

#pragma once

#include <GL/glew.h>
#include <string>

#include <NonCopyable.hpp>

namespace graphics {

  class Shader;

  /**
   * Class for initialising and managing an OpenGL shader program object.
   **/
  class ShaderProgram : public NonCopyable {
  public:
    ShaderProgram();
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
  class Shader : public NonCopyable {
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
    Shader(GLenum type, std::string source);
  
    /**
     * Dtor. Destroys the underlying OpenGL object.
     **/
    virtual ~Shader();
  
  private:
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
    FragmentShader(std::string source);
  };
  
  /**
   * Class for initialising and managing an OpenGL vertex shader object.
   **/
  class VertexShader : public Shader {
  public:
    VertexShader(std::string source);
  };
  
}
