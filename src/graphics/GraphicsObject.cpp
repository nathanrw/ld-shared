#include <graphics/GraphicsObject.hpp>

using namespace graphics;

//*****************************************************************************
GraphicsObject::GraphicsObject(GraphicsSystem& system)
  : m_system(system)
{
}

//*****************************************************************************
GraphicsSystem& GraphicsObject::graphics_system()
{
  return m_system;
}

//*****************************************************************************
const GraphicsSystem& GraphicsObject::graphics_system() const
{
  return m_system;
}
