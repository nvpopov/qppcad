R"(  
#version 330

uniform vec3 v_light_pos;
uniform vec3 v_color;
in vec3 fs_normal;
in vec3 fs_position;
out vec4 color;

void main(void){
  vec3 sp_light_pos = normalize(vec3(0, 1, 1));
  vec3 light_vector = normalize(sp_light_pos - fs_position);
  float diffuse = max(dot(fs_normal, light_vector), 0.01);
  
  vec4 ambient = vec4(0.01, 0.01, 0.01, 1.0);
  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
  vec4 linear_color =  vec4(v_color, 1.0) * diffuse;
  color = vec4(pow(linear_color.r, gamma.r),
               pow(linear_color.g, gamma.g),
               pow(linear_color.b, gamma.b), 1.0);
}
)"
