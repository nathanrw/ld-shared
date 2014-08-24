#pragma once

/**
 * Stuff for working with OpenGL buffer objects.
 *
 */

#include <GL/glew.h>

#include <graphics/GraphicsObject.hpp>

namespace graphics {
  
  /**
   * A type safe representation of an OpenGL buffer target.
   */
  enum class BufferTarget {
    ARRAY_BUFFER = GL_ARRAY_BUFFER
  };
  inline GLenum get_gl_enum(BufferTarget t) { return static_cast<GLenum>(t); }
  
  /**
   * A type safe representation of an OpenGL buffer usage.
   */
  enum class BufferUsage {
    STATIC_DRAW = GL_STATIC_DRAW,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW
  };
  inline GLenum get_gl_enum(BufferUsage u) { return static_cast<GLenum>(u); }
  
  /**
   * A type safe representation of an OpenGL data type enum.
   */
  enum class DataType {
    BYTE = GL_BYTE,
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    SHORT = GL_SHORT,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    INT = GL_INT,
    UNSIGNED_INT = GL_UNSIGNED_INT,
    HALF_FLOAT = GL_HALF_FLOAT,
    FLOAT = GL_FLOAT,
    DOUBLE = GL_DOUBLE,
    FIXED = GL_FIXED,
    INT_2_10_10_10_REV = GL_INT_2_10_10_10_REV,
    UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
    UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
  };
  inline GLenum get_gl_enum(DataType t) { return static_cast<GLenum>(t); }
  
  /**
   * A number representing the number of components making up a data item in
   * a buffer.
   */
  enum class ComponentCount {
    ONE = 1,
    TWO,
    THREE,
    FOUR
  };
  inline GLint get_gl_int(ComponentCount c) { return static_cast<GLint>(c); }

  /**
   * An OpenGL vertex buffer object.
   */
  class VertexBufferObject : public GraphicsObject {
  public:
    VertexBufferObject(
      GraphicsSystem& tok, 
      BufferTarget target, 
      BufferUsage usage
    );
    void bind();
    void fill(size_t size, void* data);
    BufferTarget target() const;
    BufferUsage usage() const;
    ~VertexBufferObject();
  private:
    BufferTarget m_target;
    BufferUsage m_usage;
    GLuint m_id;
  };
  
  /**
   * An index into a vertex array object.
   */
  class AttributeIndex {
  public:
    explicit AttributeIndex(int idx) : m_idx(idx) {}
    int idx() const { return m_idx; }
  private:
    int m_idx;
  };
  
  /**
   * An OpenGL vertex array object.
   */
  class VertexArrayObject : public GraphicsObject {
  public:
    explicit VertexArrayObject(GraphicsSystem& tok);
    void bind();
    void enable_attribute(AttributeIndex);
    void disable_attribute(AttributeIndex);
    void attribute_pointer(
      AttributeIndex idx,
      VertexBufferObject& vbo, 
      ComponentCount size, 
      DataType type, 
      bool normalized, 
      size_t stride
    );
    ~VertexArrayObject();
  private:
    GLuint m_id;
  };

}
