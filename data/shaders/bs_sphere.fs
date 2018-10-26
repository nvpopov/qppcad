R"(
#version 330 core
uniform vec3 v_color; 
smooth in vec2 fs_position;
out vec4 color;

void main(void) {
  float x = fs_position.x;
  float y = fs_position.y;
  float zz = 1.0 - x*x - y*y;
  if (zz <= 0.0) discard;

  float z = sqrt(zz);
  vec3 normal = vec3(x,y,z);
  float diffuse_term = clamp(dot(normal, vec3(0,0,1)), 0.0, 1.0);
  color = vec4(vec3(0.15,0.15,0.15) +  diffuse_term * v_color, 1.0);
}
)"
