R"(  
#version 330

uniform mat4 m_model_view;
uniform mat3 m_model_view_inv_tr;
uniform mat4 m_model_view_proj;
uniform vec3 v_line_start;
uniform vec3 v_line_end;
in vec3 vs_position;
out vec3 fs_position;
out float ax;

void main(void){
  vec3 tr_position = mix(v_line_start, v_line_end, vs_position.x);
  ax = vs_position.x * length(v_line_start-v_line_end);
  gl_Position = m_model_view_proj * vec4(tr_position, 1.0);
}
)"
