R"(  
#version 330

uniform mat4 m_model_view_proj;
uniform vec3 v_translate;

in vec3 vs_position;

void main(void){
  vec4 tr_position =  m_model_view_proj * vec4(vs_position + v_translate, 1.0);
  gl_Position = tr_position;
}
)"
