#pragma once

/**
 * Base class for all graphics objects i.e objects that mess with the opengl
 * state.
 *
 * They are not copyable since they typically hold the ID of an opengl
 * object, and hold a token from the graphics system so that they cannot
 * be constructed before it is initialised.
 */

#include <utils/NonCopyable.hpp>

namespace graphics {
  
  class GraphicsSystem;

  class GraphicsObject : public NonCopyable {
  protected:
    explicit GraphicsObject(GraphicsSystem& system);
    GraphicsSystem& graphics_system();
    const GraphicsSystem& graphics_system() const;
  private:
    GraphicsSystem& m_system; 
  };

}
