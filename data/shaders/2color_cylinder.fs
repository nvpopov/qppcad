R"(
#version 330

uniform vec3 v_light_pos;
uniform vec3 v_color1;
uniform vec3 v_color2;
uniform mat4 m_view_proj;
uniform float f_specular_intensity;
uniform float f_specular_alpha;
in vec3 fs_normal;
in vec3 fs_position;
in float cylinder_primordial_z;
out vec4 color;

void main(void){
  vec3 sp_light_pos = normalize(vec3(0, 0, 1));
  vec3 light_vector = normalize(sp_light_pos);

  vec3 rvec = normalize(vec3(0, 1, 1) + sp_light_pos);

  float diffuse = max(dot(fs_normal, light_vector), 0.05);
  float specular = max(dot(fs_normal, rvec), 0.01);
  specular = pow(specular, f_specular_intensity) * f_specular_alpha;

  vec4 ambient = vec4(0.01, 0.01, 0.01, 1.0);
  vec3 gamma = vec3(1.0/2.2, 1.0/2.2, 1.0/2.2);
  float fs_color_mix = round(cylinder_primordial_z);	
  vec3 tmp_color = v_color2*fs_color_mix + v_color1*(1-fs_color_mix);
  vec4 linear_color =  vec4(tmp_color, 1.0) * diffuse + vec4(1.0, 1.0, 1.0, 1.0) * specular;
  color = vec4(pow(linear_color.r, gamma.r),
               pow(linear_color.g, gamma.g),
               pow(linear_color.b, gamma.b), 1.0);
}
)"
