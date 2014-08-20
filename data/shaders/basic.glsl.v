#version 140
in vec3 vp;
in vec2 vt;
out vec2 texture_coordinates;
void main() {
  gl_Position = vec4 (vp, 1.0);
  texture_coordinates = vt;
}
