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

  class GraphicsToken;

  class GraphicsObject : public NonCopyable {
  protected:
    explicit GraphicsObject(GraphicsToken& token);
  private:
    GraphicsToken& m_token; 
  };

}
