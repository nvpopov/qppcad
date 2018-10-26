R"(
#version 330
uniform vec3 v_color; 
smooth in vec2 fs_position;
out vec4 color;

void main(void) {
  float x = fs_position.x;
  float y = fs_position.y;
  float zz = 1.0 - x*x - y*y;
  if (zz <= 0.0) discard;
  color = vec4(v_color, 1.0);
}
)"
