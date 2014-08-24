//*****************************************************************************
// Main

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include <filesystem/Path.hpp>
#include <graphics/GraphicsSystem.hpp>
#include <graphics/Sprite.hpp>
#include <glfwutils/glfw_utils.hpp>
#include <game/ConnectFour.hpp>

using namespace graphics;
using namespace filesystem;
using namespace Eigen;
using namespace std;
using namespace game;

//*****************************************************************************
// Main
int main(int argc, char** argv)
{
  GraphicsSystem graphics(Vector2i(276, 340), "Connect 4 With Planets");

  glDisable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  
  // Limit to 60fps.
  glfwSwapInterval(1);
  
  ConnectFour game(graphics);
  
  // Make the game listen to user input.
  graphics.window().add_event_listener(game);
    
  while (!graphics.window().should_close()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    game.update(0.1);
    game.draw();
    
    graphics.window().swap_buffers();
    glfwPollEvents();
  }

  return 0;
}
