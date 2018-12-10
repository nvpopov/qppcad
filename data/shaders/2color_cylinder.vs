R"(  
#version 330
uniform mat4 m_model_view;
uniform mat4 m_model_view_proj;
uniform mat4 m_model_view_inv_tr;
in vec3 vs_position;
in vec3 vs_normal;
out vec3 fs_normal;
out vec3 fs_position;
out float cylinder_primordial_z;

void main(void){
  cylinder_primordial_z = vs_position.z;
  fs_normal = normalize(vec3(m_model_view_inv_tr * vec4(vs_normal,0.0)));
  fs_position = vec3(m_model_view * vec4(vs_position, 1.0));
  gl_Position = m_model_view_proj * vec4(vs_position, 1.0);
}
)"
