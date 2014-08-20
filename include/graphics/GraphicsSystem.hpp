#pragma once

/**
 * Class which initialises the graphics stuff. Provides access to a graphics
 * token, which most graphics classes require as a constructor parameter - this
 * prevents them from being constructed before the graphics system (i.e. 
 * opengl, glfw etc) is initialised.
 *
 * The token doesn't actually know where it came from - so it only really makes
 * sense for there to be a single GraphicsSystem instance. This is a problem
 * for another day.
 */

#include <string>

#include <glfwutils/glfw_utils.hpp>

namespace graphics {
  
  /**
   * Graphics token - ensures the graphics system is initialised before
   * graphics objects can be constructed.
   */
  class GraphicsToken {
  private:
    friend class GraphicsSystem;
    GraphicsToken();
    ~GraphicsToken();
    GraphicsToken(const GraphicsToken&) = delete;
    GraphicsToken& operator=(const GraphicsToken&) = delete;
  };
  
  /**
   * Initialises the graphics system and provides a graphics token.
   */
  class GraphicsSystem {
  public:
    
    /**
     * Constructor. Initialise everything and open a window with the given
     * title. You can then get it by calling window().
     */
    GraphicsSystem(Size2D window_size, std::string window_title);
    
    /**
     * Get the window.
     */
    GLFWWindow& window();
    
    /**
     * Get the graphics token. Use this to initialise graphics objects.
     */
    GraphicsToken& token();
    
    ~GraphicsSystem();

  private:
    GLFWToken* m_glfw_token;
    GLFWWindow* m_window;
    GraphicsToken m_graphics_token;
  };

}
