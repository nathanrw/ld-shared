/**
 * Implementation of shader program related classes.
 **/

#include <stdexcept>

#include <graphics/ShaderProgram.hpp>

//----- Shader

graphics::Shader::Shader(GLenum type, std::string source) {

  m_id = glCreateShader(type);
  const char* program_source = source.c_str();
  glShaderSource (m_id, 1, &program_source, nullptr);
  glCompileShader(m_id);
  
  if (invalid()) {
    std::string info = info_log();
    glDeleteShader(m_id);
    throw std::runtime_error(info);
  }
}

bool graphics::Shader::invalid() {
  GLint ok;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &ok);
  return !ok;
}

std::string graphics::Shader::info_log() {
  GLint max_length;
  glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &max_length);

  char* buf = new char[max_length];
  GLsizei actual_length;
  glGetShaderInfoLog(m_id, max_length, &actual_length, buf);
  
  return std::string(buf, actual_length);
}

graphics::Shader::~Shader() { 
  glDeleteShader(m_id); 
}

graphics::FragmentShader::FragmentShader(std::string source) 
  : Shader(GL_FRAGMENT_SHADER, source) 
{
}

graphics::VertexShader::VertexShader(std::string source) 
  : Shader(GL_VERTEX_SHADER, source) 
{
}


//----- ShaderProgram

graphics::ShaderProgram::ShaderProgram() { 
  m_id = glCreateProgram(); 
}

void graphics::ShaderProgram::attach(const Shader& shader) { 
  glAttachShader(m_id, shader.m_id); 
}

bool graphics::ShaderProgram::link() { 
  glLinkProgram(m_id); 
}

void graphics::ShaderProgram::bind() { 
  glUseProgram(m_id); 
}

graphics::ShaderProgram::~ShaderProgram() { 
  glDeleteProgram(m_id); 
}
