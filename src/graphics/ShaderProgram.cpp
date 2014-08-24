/**
 * Implementation of shader program related classes.
 **/

#include <stdexcept>
#include <fstream>
#include <iostream>

#include <graphics/ShaderProgram.hpp>

using namespace graphics;
using namespace filesystem;
using namespace Eigen;

//----- Shader

//*****************************************************************************
Shader::Shader(GraphicsSystem& tok, GLenum type, std::string source) 
  : GraphicsObject(tok)
{
  initialise(type, source);
}

//*****************************************************************************
Shader::Shader(GraphicsSystem& tok, GLenum type, Path filename)
  : GraphicsObject(tok)
{
  std::ifstream ifs(filename.path());

  std::string source;
  while (ifs.good()) {
    std::string line;
    std::getline(ifs, line);
    source.append(line);
    source.append("\n");
  }

  initialise(type, source);
}

//*****************************************************************************
void Shader::initialise(GLenum type, std::string source)
{
  std::cout << "Compiling shader source:" << std::endl
	    << source << std::endl;
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

//*****************************************************************************
bool Shader::invalid() 
{
  GLint ok;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &ok);
  return !ok;
}

//*****************************************************************************
std::string Shader::info_log() 
{
  GLint max_length;
  glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &max_length);

  char* buf = new char[max_length];
  GLsizei actual_length;
  glGetShaderInfoLog(m_id, max_length, &actual_length, buf);
  
  return std::string(buf, actual_length);
}

//*****************************************************************************
Shader::~Shader() 
{ 
  glDeleteShader(m_id); 
}

//*****************************************************************************
FragmentShader::FragmentShader(GraphicsSystem& tok, std::string source) 
  : Shader(tok, GL_FRAGMENT_SHADER, source) 
{
}

//*****************************************************************************
FragmentShader::FragmentShader(GraphicsSystem& tok, Path path)
  : Shader(tok, GL_FRAGMENT_SHADER, path)
{
}

//*****************************************************************************
VertexShader::VertexShader(GraphicsSystem& tok, std::string source) 
  : Shader(tok, GL_VERTEX_SHADER, source) 
{
}

//*****************************************************************************
VertexShader::VertexShader(GraphicsSystem& tok, Path path) 
  : Shader(tok, GL_VERTEX_SHADER, path) 
{
}


//----- ShaderProgram

//*****************************************************************************
ShaderProgram::ShaderProgram(GraphicsSystem& tok) 
  : GraphicsObject(tok)
{ 
  m_id = glCreateProgram(); 
}

//*****************************************************************************
void ShaderProgram::attach(const Shader& shader) 
{ 
  glAttachShader(m_id, shader.m_id); 
}

//*****************************************************************************
bool ShaderProgram::link() 
{ 
  glLinkProgram(m_id); 
}

//*****************************************************************************
void ShaderProgram::set_uniform(std::string name, float value)
{
  bind();
  int location = glGetUniformLocation(m_id, name.c_str());
  glUniform1f(location, value);
}

//*****************************************************************************
void ShaderProgram::set_uniform(std::string name, Vector2f value)
{
  bind();
  int location = glGetUniformLocation(m_id, name.c_str());
  glUniform2f(location, value[0], value[1]);
}

//*****************************************************************************
void ShaderProgram::set_uniform(std::string name, int value)
{
  bind();
  int location = glGetUniformLocation(m_id, name.c_str());
  glUniform1i(location, value);
}

//*****************************************************************************
void ShaderProgram::set_uniform(std::string name, Vector2i value)
{
  bind();
  int location = glGetUniformLocation(m_id, name.c_str());
  glUniform2i(location, value[0], value[1]);
}

//*****************************************************************************
void ShaderProgram::bind() 
{ 
  glUseProgram(m_id); 
}

//*****************************************************************************
ShaderProgram::~ShaderProgram() 
{ 
  glDeleteProgram(m_id); 
}
