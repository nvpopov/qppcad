R"(  
#version 330
uniform sampler2D texture_0;
in vec2 fs_texcoord;
out vec4 color;
void main(void){
  color =  vec4(texture(texture_0, fs_texcoord).xyz, 1.0);
}
)"
