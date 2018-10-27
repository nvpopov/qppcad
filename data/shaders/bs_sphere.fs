R"(
#version 330 core
uniform vec3 v_color; 
uniform float f_specular_intensity;
uniform float f_specular_alpha;
smooth in vec3 fs_position;
out vec4 color;

void main(void) {
  float x = fs_position.x;
  float y = fs_position.y;
  float zz = 1.0 - x*x - y*y;
  if (zz <= 0.0) discard;

  float z = sqrt(zz);
  vec3 normal = normalize(vec3(x,y,z));
  vec3 sp_light_pos = normalize(vec3(0, 1, 1));
  vec3 light_vector = normalize(sp_light_pos - fs_position);

  float diffuse_term = clamp(dot(normal, sp_light_pos), 0.1, 1.0);

  float specular = max(dot(normal, light_vector), 0.01);
  specular = pow(specular, f_specular_intensity) * f_specular_alpha;

  vec4 ambient = vec4(0.01, 0.01, 0.01, 1.0);
  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);

  vec4 linear_color =  ambient + vec4(v_color, 1.0) * diffuse_term + vec4(1.0, 1.0, 1.0, 1.0) * specular;
  color = vec4(pow(linear_color.r, gamma.r),
               pow(linear_color.g, gamma.g),
               pow(linear_color.b, gamma.b), 1.0);
}
)"
