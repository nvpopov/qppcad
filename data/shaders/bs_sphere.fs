R"(  
#version 330
uniform vec3 v_light_pos;
uniform vec3 v_color;
uniform float f_specular_intensity;
uniform float f_specular_alpha;
in vec3 fs_normal;
in vec3 fs_position;
out vec4 color;

void main(void){
  color = vec4(1.0, 0.0, 0.0, 1.0);
}
)"
