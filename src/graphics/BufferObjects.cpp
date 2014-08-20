#include <assert.h>

#include <graphics/BufferObjects.hpp>

using namespace graphics;

VertexBufferObject::VertexBufferObject(
  GraphicsToken& tok, 
  BufferTarget target, 
  BufferUsage usage
)
  : GraphicsObject(tok),
    m_target(target),
    m_usage(usage)
{
  glGenBuffers(1, &m_id);
}

void VertexBufferObject::bind()
{
  glBindBuffer(get_gl_enum(m_target), m_id);
}

void VertexBufferObject::fill(size_t size, void* data)
{
  bind();
  glBufferData(get_gl_enum(m_target), size, data, get_gl_enum(m_usage));
}

BufferTarget VertexBufferObject::target() const
{
  return m_target;
}

BufferUsage VertexBufferObject::usage() const
{
  return m_usage;
}

VertexBufferObject::~VertexBufferObject()
{
  glDeleteBuffers(1, &m_id);
}

VertexArrayObject::VertexArrayObject(GraphicsToken& tok)
  : GraphicsObject(tok)
{
  glGenVertexArrays(1, &m_id);
}

void VertexArrayObject::bind()
{
  glBindVertexArray(m_id);
}

void VertexArrayObject::enable_attribute(AttributeIndex index)
{
  bind();
  glEnableVertexAttribArray(index.idx());
}

void VertexArrayObject::disable_attribute(AttributeIndex index)
{
  bind();
  glDisableVertexAttribArray(index.idx());
}

void VertexArrayObject::attribute_pointer(
  AttributeIndex index,
  VertexBufferObject& vbo,
  ComponentCount component_count,
  DataType data_type,
  bool normalise,
  size_t stride
)
{
  assert(vbo.target() == BufferTarget::ARRAY_BUFFER);

  bind();
  vbo.bind();

  glVertexAttribPointer(
    index.idx(), 
    get_gl_int(component_count), 
    get_gl_enum(data_type), 
    normalise,
    stride, 
    nullptr
  );
}

VertexArrayObject::~VertexArrayObject()
{
  glDeleteVertexArrays(1, &m_id);
}
