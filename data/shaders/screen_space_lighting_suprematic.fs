R"(  
#version 330
uniform vec3 v_color;
uniform float f_specular_intensity;
uniform float f_specular_alpha;
in vec3 fs_normal;
in vec3 fs_position;
out vec4 color;

void main(void){

  vec4 linear_color =  vec4(v_color, 1.0);
  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
  color = vec4(pow(linear_color.r, gamma.r),
               pow(linear_color.g, gamma.g),
               pow(linear_color.b, gamma.b), 1.0);
}
)"
