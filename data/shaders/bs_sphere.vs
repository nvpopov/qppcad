R"(  
#version 330
uniform mat4 m_model_view;
uniform mat4 m_model_view_proj;
uniform mat4 m_model_view_inv_tr;
uniform vec3 v_translate;
uniform float f_scale;
in vec3 vs_position;
in vec3 vs_normal;
out vec3 fs_normal;
out vec3 fs_position;
void main(void){
  vec3 tr_position = (vs_position * f_scale + v_translate);
  fs_normal = vec3(m_model_view_inv_tr * vec4(vs_normal,0.0));
  fs_position = vec3(m_model_view * vec4(tr_position, 1.0));
  gl_Position = m_model_view_proj * vec4(tr_position, 1.0);
}
)"
