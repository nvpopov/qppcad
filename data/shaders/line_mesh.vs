R"(  
#version 330

uniform mat4 m_model_view;
uniform mat3 m_model_view_inv_tr;
uniform mat4 m_model_view_proj;
uniform vec3 v_translate;

in vec3 vs_position;
out float fs_z;
void main(void){
  vec4 tr_position =  m_model_view_proj * vec4(vs_position + v_translate, 1.0);
  fs_z = tr_position.z ;
  gl_Position = tr_position;
}
)"
