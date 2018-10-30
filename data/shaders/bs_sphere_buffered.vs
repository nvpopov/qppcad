R"(
#version 330 core
#extension GL_EXT_gpu_shader4 : enable

uniform mat4 m_view;
uniform mat4 m_proj;

uniform samplerBuffer texture_0;
uniform samplerBuffer texture_1;

layout(location=0) in vec3 vs_position;
layout(location=1) in vec3 vs_normal;

smooth out vec3 fs_position;
out vec3 fs_color;

void main(void) {

  mat4 m_model = mat4(1.0);
  
  vec4 pos = texelFetch(texture_0, gl_InstanceID);
  m_model[3][0] = pos.x;	
  m_model[3][1] = pos.y;
  m_model[3][2] = pos.z;

  vec4 clr_s = texelFetch(texture_1, gl_InstanceID);
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
  
  vec4 P = m_model_view * vec4(clr_s.w*vs_position, 1.0f);
  fs_position = vec3(vs_position.x*2, vs_position.y*2, vs_position.z*2);
  gl_Position = m_proj * P;	
}
)"
