R"(  
#version 330
uniform vec3 v_color;
out vec4 color;
void main(void){
  color = vec4(v_color, 1);
}
)"
