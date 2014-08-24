#include <assert.h>

#include <graphics/GraphicsSystem.hpp>
#include <graphics/Sprite.hpp>

#include <iostream>

#include <cstdlib>

using namespace graphics;
using namespace filesystem;
using namespace Eigen;
using namespace std;

//*****************************************************************************
Animation::Animation(
  GraphicsSystem& gtok,
  filesystem::Path texture_path,
  Vector2i frame_size,
  int frame_count,
  float period // Milliseconds per frame
)
// Initialise everything. This is a behemoth of an object, but it's mostly just
// OpenGL state wrangling. Conceptually, it's quite simple - a texture, a 
// rectangle and a shader to draw it with.
//*****************************************************************************
  : GraphicsObject(gtok),
    m_frame_size(frame_size),
    m_frame_count(frame_count),
    m_period(period),
    m_positions(gtok, BufferTarget::ARRAY_BUFFER, BufferUsage::STATIC_DRAW),
    m_positions_attribute(0),
    m_vertex_attributes(gtok),
    m_fragment_shader(gtok, Path("data/shaders/animation.glsl.f")),
    m_vertex_shader(gtok, Path("data/shaders/animation.glsl.v")),
    m_shader_program(gtok),
    m_texture(gtok, TextureTarget::TEXTURE_2D, texture_path)
{
  m_positions_arr[0] = Vector2f(0, 0);
  m_positions_arr[1] = Vector2f(0, frame_size[1]);
  m_positions_arr[2] = Vector2f(frame_size[0], 0);
  m_positions_arr[3] = Vector2f(frame_size[0], frame_size[1]);
  
  m_positions.fill(sizeof(m_positions_arr), m_positions_arr);
  
  m_vertex_attributes.enable_attribute(m_positions_attribute);
  m_vertex_attributes.attribute_pointer(
    m_positions_attribute,
    m_positions,
    ComponentCount::TWO,
    DataType::FLOAT,
    false,
    0
  );
  
  m_shader_program.attach(m_vertex_shader);
  m_shader_program.attach(m_fragment_shader);
  m_shader_program.link();
  
  m_shader_program.set_uniform("texture_size", m_texture.size());
  m_shader_program.set_uniform("frame_size", m_frame_size);
  m_shader_program.set_uniform("window_size", graphics_system().window_size());
  
  check_validity();
}

//*****************************************************************************
void Animation::draw(int frame, Vector2f position, float orientation_radians)
{
  assert(frame >= 0 && frame < m_frame_count);

  m_texture.bind(TextureTarget::TEXTURE_2D);
  m_shader_program.bind();
  m_vertex_attributes.bind();
  
  m_shader_program.set_uniform("frame", frame);
  m_shader_program.set_uniform("origin", position);
  m_shader_program.set_uniform("orientation", orientation_radians);
  
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//*****************************************************************************
void Animation::check_validity() const
{
  assert(m_frame_size[0] != 0);
  assert(m_frame_size[1] != 0);
  
  int num_columns = m_texture.size()[0] / m_frame_size[0];
  assert(num_columns > 0);

  int num_rows = m_texture.size()[1] / m_frame_size[1];
  assert(num_rows > 0);

  int num_rows_needed = m_frame_count / num_columns;
  assert(num_rows_needed > 0);
  assert(num_rows_needed <= num_rows);
}

//*****************************************************************************
Vector2i Animation::size() const
{
  return m_frame_size;
}

//*****************************************************************************
int Animation::frame_count() const
{
  return m_frame_count;
}

//*****************************************************************************
float Animation::period() const
{
  return m_period;
}

//*****************************************************************************
Sprite::Sprite()
  : m_time_accumulated(0),
    m_frame(0),
    m_animation(0),
    m_position(0, 0),
    m_orientation(0),
    m_animating(true)
{
}

//*****************************************************************************
void Sprite::set_animation(shared_ptr<Animation> animation)
{
  m_animation = animation;
  m_time_accumulated = 0;
  m_frame = 0;
}

//*****************************************************************************
Vector2f Sprite::position() const
{
  return m_position;
}

//*****************************************************************************
void Sprite::set_position(Vector2f position)
{
  m_position = position;
}

//*****************************************************************************
float Sprite::orientation() const
{
  return m_orientation;
}

//*****************************************************************************
void Sprite::set_orientation(float orientation)
{
  m_orientation = orientation;
}

//*****************************************************************************
void Sprite::update(float dt)
{
  if (m_animation && m_animating) {
    m_time_accumulated += dt;
    while (m_time_accumulated > m_animation->period()) {
      m_time_accumulated -= m_animation->period();
      m_frame = (m_frame + 1) % m_animation->frame_count();
    }
  }
}

//*****************************************************************************
void Sprite::draw()
{
  if (m_animation) {
    m_animation->draw(m_frame, m_position, m_orientation);
  }
}

//*****************************************************************************
bool Sprite::contains(Vector2f point) const
{
  Vector2f rel = point - m_position;
  Vector2i size = m_animation->size();

  return rel[0] >= 0 && rel[1] >= 0 && rel[0] <= size[0] && rel[1] <= size[1];
}

//*****************************************************************************
void Sprite::randomise_frame()
{
  m_frame = rand() % m_animation->frame_count();
}

//*****************************************************************************
void Sprite::stop_animating()
{
  m_animating = false;
}

//*****************************************************************************
void Sprite::set_frame(int frame)
{
  m_frame = frame;
}
