//*****************************************************************************
// Stuff for sprites.
//

#pragma once

#include <memory>

#include <Eigen/Dense>

#include <graphics/GraphicsObject.hpp>
#include <graphics/Texture.hpp>
#include <graphics/BufferObjects.hpp>
#include <graphics/ShaderProgram.hpp>

namespace graphics {
  
  //***************************************************************************
  // Wraps up various bits of rendering states under a simple interface - draw
  // a textured rectangle at a transformed position.
  class Animation : public GraphicsObject {
  public:

    Animation(
      GraphicsSystem& gtok,
      filesystem::Path texture_path,
      Eigen::Vector2i frame_size,
      int frame_count,
      float period // Milliseconds per frame
    );
    // Constructor. Takes the path to a texture on the filesystem and some
    // data. The width and height define the width and height of each frame
    // of the animation. The texture is then taken to be a series of frames
    // like so:
    //  _____________________
    // |     | ^   |     |xxx|
    // |  1  | h 2 |  3  |xxx|
    // |_____|_v___|_____|xxx|
    // |< w >|frame|xxxxxxxxx|
    // |  4  |count|xxxxxxxxx|
    // |_____|_____|xxxxxxxxx|
    //
    // Throws a runtime error if the size and frame count don't fit in the
    // texture that gets loaded. Because fuck you.

    void draw(int frame, Eigen::Vector2f position, float orientation_radians);
    // Draw the animation at the given frame. It is drawn as a rectangle
    // with min point (0, 0) and max point (width, height). The rectangle is
    // transformed such that it is centred on the given point. It is rotated
    // about its centre by the given angle.
    
    Eigen::Vector2i size() const;
    // Get the dimensions of a frame.
    
    float period() const;
    // Get the period of the animation - the number of milliseconds in a
    // frame.
    
    int frame_count() const;

  private:
    
    void check_validity() const;
    // Check the validity.
    
    Eigen::Vector2i m_frame_size;
    int m_frame_count;
    float m_period;
    // Animation frame data.

    Eigen::Vector2f m_positions_arr[4];
    VertexBufferObject m_positions;
    AttributeIndex m_positions_attribute;
    // Positions of the four vertices of the (untransformed) rectangle.

    VertexArrayObject m_vertex_attributes;
    // Vertex array object for wrapping up the above attributes.
    
    FragmentShader m_fragment_shader;
    VertexShader m_vertex_shader;
    ShaderProgram m_shader_program;
    // A simple shader program for doing the drawing.

    Texture m_texture;
    // The texture containing the frames.
  };
  

  
  //***************************************************************************
  // A thing with a position and an animation which knows how to update itself
  // and draw itself.
  class Sprite {
  public:

    Sprite();
    // Ctor. The sprite is at (0, 0) and has no animation.
    
    void stop_animating();
    // Stop animating the sprite.

    void set_animation(std::shared_ptr<Animation> animation);
    // Set an animation for the sprite.

    Eigen::Vector2f position() const;
    void set_position(Eigen::Vector2f position);
    // The position the sprite is drawn at.
    
    float orientation() const;
    void set_orientation(float orientation);

    void update(float dt);
    // Update the sprite, passing in the system time and the delta since the
    // last update.
    
    void draw();
    // Draw the sprite.
    
    bool contains(Eigen::Vector2f point) const;
    // Does the sprite contain the point?
    
    void randomise_frame();
    // Set the frame to a random value.
    
    void set_frame(int frame);
    // Set the frame.
    
  private:

    float m_time_accumulated;
    int m_frame;
    std::shared_ptr<Animation> m_animation;
    // Animation plus some state for deciding what frame to draw. Time is
    // accumulated, and when we reach the period of the animation we
    // increment the frame counter (modulo the number of frames) and subtract
    // the period from the accumulator.

    Eigen::Vector2f m_position;
    // Sprite position
    
    float m_orientation;
    // Sprite orientation.
    
    bool m_animating;

  };

}
