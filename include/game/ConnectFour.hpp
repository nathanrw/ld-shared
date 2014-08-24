#pragma once

#include <memory>

#include <Eigen/Dense>

#include <graphics/GraphicsSystem.hpp>
#include <graphics/Sprite.hpp>
#include <glfwutils/glfw_utils.hpp>

namespace game {

  //***************************************************************************
  // Thing to get animations from.
  class AnimationLibrary : public graphics::GraphicsObject {
  public:
    explicit AnimationLibrary(graphics::GraphicsSystem& system);
    std::shared_ptr<graphics::Animation> test() const;
    std::shared_ptr<graphics::Animation> blue_planet() const;
    std::shared_ptr<graphics::Animation> red_planet() const;
    std::shared_ptr<graphics::Animation> lightning() const;
    std::shared_ptr<graphics::Animation> add_button() const;
    std::shared_ptr<graphics::Animation> messages() const;
    ~AnimationLibrary();
  };
  
  //***************************************************************************
  class GameObject {
  public:
    virtual void update(float dt) =0;
    virtual void draw() =0;
    virtual ~GameObject();
  };
  
  //***************************************************************************
  // Identifies a button to its event listeners.
  class ButtonPressActionID {
  public:
    ButtonPressActionID();
    bool operator==(ButtonPressActionID id) const;
  private:
    int m_id;
  };
  
  //***************************************************************************
  // Listens for button presses.
  class ButtonPressListener {
  public:
    virtual void button_pressed(ButtonPressActionID id) =0;
  };
  
  //***************************************************************************
  // Handles mouse clicks.
  class Button : public GameObject {
  public:
    explicit Button(
      ButtonPressActionID m_event_id, 
      Eigen::Vector2f position,
      std::shared_ptr<graphics::Animation> animation
    );
    void add_listener(ButtonPressListener& listener);
    void handle_mouse_click(Eigen::Vector2f click_position);
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual ~Button();
  private:
    std::vector<ButtonPressListener*> m_listeners;
    graphics::Sprite m_sprite;
    ButtonPressActionID m_id;
  };
  
  //***************************************************************************
  // Something which drops onto the screen
  class Dropper : public GameObject {
  public:
    Dropper(
      Eigen::Vector2f start_position, 
      Eigen::Vector2f end_position
    );
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual ~Dropper();
  protected:
    graphics::Sprite& sprite();
  private:
    graphics::Sprite m_sprite;
    Eigen::Vector2f m_end_position;
  };

  enum class PlanetColour { RED, BLUE };

  //***************************************************************************
  // A planet.
  class Planet : public Dropper {
  public:
    Planet(
      PlanetColour colour, 
      AnimationLibrary& animations, 
      Eigen::Vector2f start_position, 
      Eigen::Vector2f end_position
    );
    PlanetColour colour() const;
    virtual ~Planet();
  private:
    PlanetColour m_colour;
  };
  
  //***************************************************************************
  // A message on the screen.
  class Message : public Dropper {
  public:
    Message(
      AnimationLibrary& animations, 
      Eigen::Vector2f start_position, 
      Eigen::Vector2f end_position
    );
    void player_1_wins();
    void player_2_wins();
    void is_draw();
    virtual ~Message();
  };
  
  //***************************************************************************
  // The connect four game.
  class ConnectFour : public GLFWEventListener,
		      public ButtonPressListener,
		      public graphics::GraphicsObject {
  public:
    
    ConnectFour(graphics::GraphicsSystem& graphics);
    
    //***** GLFWEventListener overrides.
    // Handle top level events - dispatch mouse clicks to buttons.
    virtual void on_mouse_moved(Eigen::Vector2d new_mouse_position) override;
    virtual void on_mouse_button_down(int button, GLFWModifierKeys mods) override;
    virtual void on_mouse_button_up(int button, GLFWModifierKeys mods) override;
    
    //***** ButtonPressListener overrides
    // Handles actions from buttons.
    virtual void button_pressed(ButtonPressActionID id);
    
    void update(float dt);
    // Update the game state in response to time.
    
    void draw();
    // Draw the game state on the screen.  
  
  private:
    
    void add_planet(int column);
    
    bool check_line(
      Eigen::Vector2i start, 
      Eigen::Vector2i gradient, 
      PlanetColour colour
    );
    
    bool check_won(PlanetColour colour);
    
    void victory();
    // The game is over.
    
    void create_message();
    
    Eigen::Vector2f screen_size() const;
    
    Eigen::Vector2f column_top(int i);
    Eigen::Vector2f column_bottom(int i);
    
    AnimationLibrary m_animations;
    // Library of animations.

    std::unique_ptr<Planet> m_planets[16];
    // The 4x4 connect 4 grid.
    
    int m_num_planets;
    
    bool m_game_over;

    ButtonPressActionID m_button_ids[4];
    std::unique_ptr<Button> m_row_buttons[4];
    // Button at the bottom of each row. Clicking on one causes a planet to
    // be dropped.
    
    std::unique_ptr<Message> m_message;
    
    Eigen::Vector2d m_mouse_pos;
    // Mouse location.
    
    PlanetColour m_turn;
    // Who's turn is it?

  };
}
