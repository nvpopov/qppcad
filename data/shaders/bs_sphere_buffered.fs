R"(
#version 330 core

uniform float f_specular_intensity;
uniform float f_specular_alpha;

smooth in vec3 fs_position;
smooth in vec3 fs_color;

out vec4 color;

void main(void) {
  float x = fs_position.x;
  float y = fs_position.y;
  float zz = 1.0 - x*x - y*y;
  if (zz <= 0.0) discard;

  float z = sqrt(zz);
  vec3 normal = vec3(x,y,z);

  float diffuse_term = clamp(dot(normal, vec3(0, 0, 1)), 0.1, 1.0);

  vec3 sp_light_pos = normalize(vec3(0, 1, 1));
  vec3 light_vector = normalize(sp_light_pos - fs_position);

  float specular = max(dot(normal, light_vector), 0.01);
  specular = pow(specular, f_specular_intensity) * f_specular_alpha;
  vec4 ambient = vec4(0.11, 0.11, 0.11, 1.0);

  color =  ambient + vec4(fs_color, 1.0) * diffuse_term + vec4(1.0, 1.0, 1.0, 1.0) * specular;
   
}
)"
