R"(
#version 330

uniform vec3 v_light_pos;
uniform vec3 v_color1;
uniform vec3 v_color2;
in vec3 fs_normal;
in vec3 fs_position;
in float cylinder_primordial_z;
out vec4 color;

void main(void){

  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
  float fs_color_mix = round(cylinder_primordial_z);	
  vec3 tmp_color = v_color2*fs_color_mix + v_color1*(1-fs_color_mix);
  vec4 linear_color =  vec4(tmp_color, 1.0);
  color = vec4(pow(linear_color.r, gamma.r),
               pow(linear_color.g, gamma.g),
               pow(linear_color.b, gamma.b), 1.0);
}
)"
