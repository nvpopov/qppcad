R"(
#version 330 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_EXT_gpu_shader4 : enable

uniform mat4 m_view;
uniform mat4 m_proj;

uniform samplerBuffer pos_buf;
uniform samplerBuffer clr_s_buf;

layout(location=0) in vec3 vs_position;
layout(location=1) in vec3 vs_normal;

smooth out vec3 fs_position;
smooth out vec3 fs_color;

void main(void) {

  mat4 m_model = mat4(1.0);
  
  vec4 pos = texelFetchBuffer(pos_buf, gl_InstanceID);
  m_model[3][0] = pos[0];	
  m_model[3][1] = pos[1];
  m_model[3][2] = pos[2];

  vec4 clr_s = texelFetchBuffer(clr_s_buf, gl_InstanceID);
  fs_color = clr_s.xyz;

  mat4 m_model_view = m_view * m_model;

  // First colunm.
  m_model_view[0][0] = 1.0; 
  m_model_view[0][1] = 0.0; 
  m_model_view[0][2] = 0.0; 

  // Second colunm.
  m_model_view[1][0] = 0.0; 
  m_model_view[1][1] = 1.0; 
  m_model_view[1][2] = 0.0; 

  // Thrid colunm.
  m_model_view[2][0] = 0.0; 
  m_model_view[2][1] = 0.0; 
  m_model_view[2][2] = 1.0; 
  
  vec4 P = m_model_view * vec4(clr_s[3]*vs_position, 1.0f);
  fs_position = vec3(vs_position.x*2, vs_position.y*2, vs_position.z*2);
  gl_Position = m_proj * P;	
}
)"
