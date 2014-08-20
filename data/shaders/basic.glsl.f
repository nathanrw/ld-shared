#version 140
in vec2 texture_coordinates;
uniform sampler2D texture;
out vec4 frag_colour;
void main() {
  frag_colour = texture2D(texture, texture_coordinates);
}
