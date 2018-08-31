R"(  
#version 330

uniform vec3 v_color;
in float fs_z;
out vec4 color;
void main(void){
  float effect_alpha = exp(-0.6 * max(0.0, 76- fs_z));
  vec4 bg_color = vec4(0.4, 0.4, 0.4, 1.0);
  color = mix(vec4(v_color, 1), bg_color, effect_alpha);
}
)"
