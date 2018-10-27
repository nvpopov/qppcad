R"(
#version 330 core

uniform mat4 m_model_view;
uniform mat4 m_proj;
uniform float f_scale;

layout(location=0) in vec3 vs_position;
layout(location=1) in vec3 vs_normal;

smooth out vec3 fs_position;

void main(void){
  mat4 modelView = m_model_view;
  
  // First colunm.
  modelView[0][0] = 1.0; 
  modelView[0][1] = 0.0; 
  modelView[0][2] = 0.0; 


  // Second colunm.
  modelView[1][0] = 0.0; 
  modelView[1][1] = 1.0; 
  modelView[1][2] = 0.0; 

  // Thrid colunm.
  modelView[2][0] = 0.0; 
  modelView[2][1] = 0.0; 
  modelView[2][2] = 1.0; 
  
  vec4 P = modelView * vec4(f_scale*vs_position, 1.0f);
  fs_position = vec3(vs_position.x*2, vs_position.y*2, vs_position.z*2);
  gl_Position = m_proj * P;	
}
)"
