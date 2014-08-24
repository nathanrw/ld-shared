#pragma once

/**
 * Class which initialises the graphics stuff. 
 */

#include <string>

#include <Eigen/Dense>

#include <glfwutils/glfw_utils.hpp>

namespace graphics {
  
  /**
   * Initialises the graphics system
   */
  class GraphicsSystem {
  public:
    
    GraphicsSystem(Eigen::Vector2i window_size, std::string window_title);
    // Constructor. Initialise everything and open a window with the given
    // title. You can then get it by calling window().
    
    Eigen::Vector2i window_size() const;
    // Get the window size.
    
    GLFWWindow& window();
    // Get the window.
    
    ~GraphicsSystem();

  private:
    GLFWToken* m_glfw_token;
    GLFWWindow* m_window;
  };

}
