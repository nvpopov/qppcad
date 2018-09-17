R"(  
#version 330
uniform sampler2D texture_color;
in vec2 fs_texcoord;
out vec4 color;
void main(void){
  color = texture(texture_color, fs_texcoord);
}
)"
