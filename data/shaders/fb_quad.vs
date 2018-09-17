R"(  
#version 330
in vec3 vs_position;
in vec3 vs_normal;
out vec2 fs_texcoord;
void main(void){
  fs_texcoord = vs_normal.xy;
  gl_Position = vec4(vs_position, 1.0);
}
)"
