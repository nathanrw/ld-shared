#include <game/ConnectFour.hpp>

#include <filesystem/Path.hpp>

#include <iostream>

using namespace Eigen;
using namespace graphics;
using namespace game;
using namespace std;
using namespace filesystem;

static int s_animation_library_instance_count = 0;
static shared_ptr<Animation> s_test;
static shared_ptr<Animation> s_blue_planet;
static shared_ptr<Animation> s_red_planet;
static shared_ptr<Animation> s_lightning;
static shared_ptr<Animation> s_add_button;
static shared_ptr<Animation> s_messages;

//*****************************************************************************
AnimationLibrary::AnimationLibrary(GraphicsSystem& system)
  : GraphicsObject(system)
{
  if (s_animation_library_instance_count++ == 0) {
    s_test = make_shared<Animation>(
      system, 
      Path("data/textures/test_animation.png"), 
      Vector2i(64, 64), 
      16, 
      1
    );
    s_blue_planet = make_shared<Animation>(
      system, 
      Path("data/textures/planet.png"), 
      Vector2i(64, 64), 
      16, 
      1
    );
    s_red_planet = make_shared<Animation>(
      system, 
      Path("data/textures/red_planet.png"), 
      Vector2i(64, 64), 
      16, 
      1
    );
    s_lightning = make_shared<Animation>(
      system, 
      Path("data/textures/lightning.png"), 
      Vector2i(86, 18), 
      8, 
      1
    );
    s_add_button = make_shared<Animation>(
      system, 
      Path("data/textures/add_button.png"), 
      Vector2i(64, 64), 
      16, 
      0.4
    );
    s_messages = make_shared<Animation>(
      system,
      Path("data/textures/messages.png"),
      Vector2i(256, 32),
      3,
      8
    );
  }
}

//*****************************************************************************
AnimationLibrary::~AnimationLibrary()
{
  if (--s_animation_library_instance_count == 0) {
    s_test.reset();
    s_blue_planet.reset();
    s_red_planet.reset();
    s_lightning.reset();
    s_add_button.reset();
    s_messages.reset();
  }
}

//*****************************************************************************
shared_ptr<Animation> AnimationLibrary::test() const
{
  return s_test;
}

//*****************************************************************************
shared_ptr<Animation> AnimationLibrary::blue_planet() const
{
  return s_blue_planet;
}

//*****************************************************************************
shared_ptr<Animation> AnimationLibrary::red_planet() const
{
  return s_red_planet;
}

//*****************************************************************************
shared_ptr<Animation> AnimationLibrary::lightning() const
{
  return s_lightning;
}

//*****************************************************************************
shared_ptr<Animation> AnimationLibrary::add_button() const
{
  return s_add_button;
}

//*****************************************************************************
shared_ptr<Animation> AnimationLibrary::messages() const
{
  return s_messages;
}

//*****************************************************************************
// GameObject
//*****************************************************************************

GameObject::~GameObject() {}

//*****************************************************************************
// Buttons
//*****************************************************************************

static int s_next_button_id = 0;

//*****************************************************************************
ButtonPressActionID::ButtonPressActionID()
  : m_id(s_next_button_id++)
{
}

//*****************************************************************************
bool ButtonPressActionID::operator==(ButtonPressActionID that) const
{
  return m_id == that.m_id;
}

//*****************************************************************************
Button::Button(
  ButtonPressActionID id, 
  Vector2f position, 
  shared_ptr<Animation> animation
)
  : m_id(id)
{
  m_sprite.set_position(position);
  m_sprite.set_animation(animation);
  m_sprite.randomise_frame();
}

//*****************************************************************************
void Button::add_listener(ButtonPressListener& listener)
{
  m_listeners.push_back(&listener);
}

//*****************************************************************************
void Button::handle_mouse_click(Vector2f mouse_click_position)
{
  if (m_sprite.contains(mouse_click_position)) {
    for (auto& listener : m_listeners) {
      cout << "ffff";
      listener->button_pressed(m_id);
    }
  }
}

//*****************************************************************************
void Button::update(float dt)
{
  m_sprite.update(dt);
}

//*****************************************************************************
void Button::draw()
{
  m_sprite.draw();
}

//*****************************************************************************
Button::~Button() 
{}

//*****************************************************************************
// Dropper
//*****************************************************************************

//*****************************************************************************
Dropper::Dropper(Vector2f start_position, Vector2f end_position)
  : m_end_position(end_position)
{
  m_sprite.set_position(start_position);
}

//*****************************************************************************
void Dropper::update(float dt)
{
  Vector2f pos = m_sprite.position();
  if (pos != m_end_position) {
    m_sprite.set_position(pos + dt * (m_end_position - pos));
  }
  m_sprite.update(dt);
}

//*****************************************************************************
Sprite& Dropper::sprite()
{
  return m_sprite;
}

//*****************************************************************************
void Dropper::draw()
{
  m_sprite.draw();
}

//*****************************************************************************
Dropper::~Dropper() {}

//*****************************************************************************
// Planet
//*****************************************************************************

//*****************************************************************************
Planet::Planet(
  PlanetColour colour, 
  AnimationLibrary& animations, 
  Vector2f start_position, 
  Vector2f end_position
)
  : Dropper(start_position, end_position),
    m_colour(colour)
{
  if (colour == PlanetColour::BLUE) {
    sprite().set_animation(animations.blue_planet());
  } else {
    sprite().set_animation(animations.red_planet());
  }
}

//*****************************************************************************
PlanetColour Planet::colour() const
{
  return m_colour;
}

//*****************************************************************************
Planet::~Planet() {}

//*****************************************************************************
// Message
//*****************************************************************************

Message::Message(
  AnimationLibrary& animations, 
  Vector2f start_position, 
  Vector2f end_position
)
  : Dropper(start_position, end_position)
{
  sprite().set_animation(animations.messages());
  sprite().stop_animating();
}

//*****************************************************************************
void Message::player_1_wins()
{
  sprite().set_frame(0);
}

//*****************************************************************************
void Message::player_2_wins()
{
  sprite().set_frame(1);
}

//*****************************************************************************
void Message::is_draw()
{
  sprite().set_frame(2);
}

//*****************************************************************************
Message::~Message()
{
}

//*****************************************************************************
// Game
//*****************************************************************************

//*****************************************************************************
ConnectFour::ConnectFour(GraphicsSystem& graphics)
  : GraphicsObject(graphics),
    m_animations(graphics),
    m_turn(PlanetColour::BLUE),
    m_game_over(false),
    m_num_planets(0)
{
  // Initialise the buttons.
  auto anim = m_animations.add_button();
  for (int i = 0; i < 4; ++i) {
    Vector2f pos = column_bottom(i) + Vector2f(0, 64);
    m_row_buttons[i].reset(new Button(m_button_ids[i], pos, anim));
    m_row_buttons[i]->add_listener(*this);
  }
}

//*****************************************************************************
void ConnectFour::on_mouse_moved(Vector2d pos)
{
  m_mouse_pos = pos;
}

//*****************************************************************************
void ConnectFour::on_mouse_button_down(int button, GLFWModifierKeys keys)
{
}

//*****************************************************************************
void ConnectFour::on_mouse_button_up(int button, GLFWModifierKeys keys)
{
  for (auto& button : m_row_buttons) {
    button->handle_mouse_click(m_mouse_pos.cast<float>());
  }
}

//*****************************************************************************
void ConnectFour::button_pressed(ButtonPressActionID id) 
{
  for (int i = 0; i < 4; ++i) {
    if (id == m_button_ids[i]) {
      add_planet(i);
      break;
    }
  }
}

//*****************************************************************************
void ConnectFour::add_planet(int column)
{
  if (m_game_over) {
    return;
  }

  for (int row = 0; row < 4; ++row) {
    int cell = row * 4 + column;
    if (!m_planets[cell]) {
      m_planets[cell].reset(
        new Planet(
          m_turn, 
          m_animations, 
          column_top(column), 
          column_bottom(column) 
        )
      );
      
      ++m_num_planets;

      // Swap players.
      if (m_turn == PlanetColour::RED) {
	m_turn = PlanetColour::BLUE;
      } else {
	m_turn = PlanetColour::RED;
      }
      break;
    }
  }
  
  // Check for game over.
  victory();
}

//*****************************************************************************
bool ConnectFour::check_line(Vector2i start, Vector2i gradient, PlanetColour colour)
{
  Vector2i point = start;
  for (int i = 0; i < 4; ++i) {
    int cell = point[1]*4+point[0];
    if (!m_planets[cell] || m_planets[cell]->colour() != colour) {
      return false;
    }
    point += gradient;
  }
  return true;
}

//*****************************************************************************
bool ConnectFour::check_won(PlanetColour player)
{
  for (int i = 0; i < 4; ++i) {
    if (check_line(Vector2i(0, i), Vector2i(1, 0), player)) return true;
  }

  for (int i = 0; i < 4; ++i) {
    if (check_line(Vector2i(i, 0), Vector2i(0, 1), player)) return true;
  }
  
  return 
    check_line(Vector2i(0, 0), Vector2i(1, 1), player) ||
    check_line(Vector2i(0, 3), Vector2i(1, -1), player);
}

//*****************************************************************************
void ConnectFour::victory()
{
  if (check_won(PlanetColour::BLUE)) {
    create_message();
    m_message->player_1_wins();
    m_game_over = true;
  } else if (check_won(PlanetColour::RED)) {
    create_message();
    m_message->player_2_wins();
    m_game_over = true;
  } else if (m_num_planets == 16) {
    create_message();
    m_message->is_draw();
    m_game_over = true;
  }
}

//*****************************************************************************
void ConnectFour::update(float dt)
{
  for (auto& ptr : m_planets) {
    if (ptr) {
      ptr->update(dt);
    }
  }
  
  for (auto& ptr : m_row_buttons) {
    if (ptr) {
      ptr->update(dt);
    }
  }
  
  if (m_message) {
    m_message->update(dt);
  }
}

//*****************************************************************************
void ConnectFour::draw()
{
  for (auto& ptr : m_planets) {
    if (ptr) {
      ptr->draw();
    }
  }
  
  for (auto& ptr : m_row_buttons) {
    if (ptr) {
      ptr->draw();
    }
  }
  
  if (m_message) {
    m_message->draw();
  }
}

//*****************************************************************************
Vector2f ConnectFour::column_top(int i)
{
  return Vector2f(10 + 64 * i, 0);
}

//*****************************************************************************
Vector2f ConnectFour::column_bottom(int i)
{
  float x = 10 + 64 * i;
  float y = 340 - (64 * 2 + 10);
  for (int row = 0; row < 4; ++row) {
    int cell = row*4 + i;
    if (m_planets[cell]) {
      y -= 64;
    }
  }
  return Vector2f(x, y);
}

//*****************************************************************************
void ConnectFour::create_message()
{
  auto wh = screen_size();
  float w = (wh[0] - 256) / 2;
  float h = (wh[1] - 32) / 2 - 32;
  m_message.reset(new Message(m_animations, Vector2f(w, 0), Vector2f(w, h)));
}

//*****************************************************************************
Vector2f ConnectFour::screen_size() const
{
  return graphics_system().window_size().cast<float>();
}
