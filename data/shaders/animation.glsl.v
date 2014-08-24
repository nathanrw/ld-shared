#version 140

in vec2 position;
// 2D position in rect [origin, texture_size]

uniform ivec2 window_size;
uniform ivec2 texture_size;
uniform ivec2 frame_size;
// Texture and frame sizes. These are in pixels.

uniform int frame;
uniform vec2 origin;
uniform float orientation;

out vec2 texcoords;

//****************************************************************************/
void main() {

  // Calculate the texture coordinates.
  int frames_per_row = texture_size.x / frame_size.x;

  int column = frame % frames_per_row;
  int row = frame / frames_per_row;

  vec2 offset = vec2(column, row) * frame_size;
  
  texcoords = (position + offset) / texture_size;

  vec2 world_pos = position + origin;
  world_pos[1] = window_size[1] - world_pos[1];
  vec2 screen_pos = (world_pos - window_size/2) / (window_size/2);

  gl_Position = vec4(screen_pos, 0.0, 1.0);
}
