#version 140

in vec2 texcoords;
uniform sampler2D tex;
out vec4 frag_colour;

void main() {
  frag_colour = texture(tex, texcoords);
}
