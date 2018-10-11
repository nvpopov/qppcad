R"(  
#version 330
uniform vec3 v_color;
in vec3 fs_position;
in float ax;
out vec4 color;
void main(void){
  if (fract(ax*2) > 0.4 && fract(ax*2) < 0.9 ) discard;
  color = vec4(v_color, 1.0);
}
)"
