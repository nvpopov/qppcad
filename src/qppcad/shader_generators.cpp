#include <qppcad/shader_generators.hpp>

using namespace qpp;
using namespace qpp::cad;


shader_program_t *shader_generators::gen_default_program () {
  std::string vs =
    #include "shaders/screen_space_lighting.vs"
      ;

  std::string fs =
    #include "shaders/screen_space_lighting.fs"
      ;

  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(std::string("default_program"),
                                                                  vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_model_view);
  sp->u_on(sp_u_name::m_model_view_inv_tr);
  //sp->u_on(sp_u_name::v_light_pos);
  //sp->u_on(sp_u_name::v_eye_pos);
  sp->u_on(sp_u_name::f_specular_intensity);
  sp->u_on(sp_u_name::f_specular_alpha);
  sp->u_on(sp_u_name::v_translate);
  sp->u_on(sp_u_name::f_scale);
  sp->u_on(sp_u_name::v_color);
  return sp;
}

shader_program_t *shader_generators::gen_unit_line_program () {
  std::string vs =
    #include "shaders/unit_line.vs"
      ;

  std::string fs =
    #include "shaders/unit_line.fs"
      ;

  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(std::string("unit_line_program"),
                                                                  vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
//  sp->u_on(sp_u_name::m_view);
//  sp->u_on(sp_u_name::m_view_inv_tr);
  sp->u_on(sp_u_name::v_color);
  sp->u_on(sp_u_name::v_line_start);
  sp->u_on(sp_u_name::v_line_end);
  return sp;
}

shader_program_t *shader_generators::gen_unit_line_styled_program () {
  std::string vs =
    #include "shaders/unit_line_styled.vs"
      ;

  std::string fs =
    #include "shaders/unit_line_styled.fs"
      ;

  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(std::string("unit_line_styled"),
                                                                  vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  //sp->u_on(sp_u_name::m_view);
  //sp->u_on(sp_u_name::m_view_inv_tr);
  sp->u_on(sp_u_name::v_color);
  sp->u_on(sp_u_name::v_line_start);
  sp->u_on(sp_u_name::v_line_end);
  return sp;
}

shader_program_t *shader_generators::gen_line_mesh_program () {
  std::string vs =
    #include "shaders/line_mesh.vs"
      ;

  std::string fs =
    #include "shaders/line_mesh.fs"
      ;

  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(std::string("grid_program"),
                                                                  vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::v_translate);
  sp->u_on(sp_u_name::v_color);
  return sp;
}

shader_program_t *shader_generators::gen_mv_screen_space_lighting_program () {
  std::string vs =
    #include "shaders/mv_screen_space_lighting.vs"
      ;

  std::string fs =
    #include "shaders/mv_screen_space_lighting.fs"
      ;

  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(std::string("mv_ssl"), vs, fs);
  sp->u_on(sp_u_name::m_model_view_proj);
  sp->u_on(sp_u_name::m_model_view);
  //sp->u_on(sp_u_name::m_view_proj);
  sp->u_on(sp_u_name::f_specular_intensity);
  sp->u_on(sp_u_name::f_specular_alpha);
  sp->u_on(sp_u_name::m_model_view_inv_tr);
  //sp->u_on(sp_u_name::v_light_pos);
  sp->u_on(sp_u_name::v_color);

  return sp;
}

shader_program_t *shader_generators::gen_fbo_quad_program () {
  std::string vs =
    #include "shaders/fb_quad.vs"
      ;

  std::string fs =
    #include "shaders/fb_quad.fs"
      ;
  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(
                                     std::string("fbo_quad"), vs, fs);
  sp->u_on(sp_u_name::texture_0);
  return sp;
}

shader_program_t *shader_generators::gen_bs_sphere_program () {
  std::string vs =
    #include "shaders/bs_sphere.vs"
      ;

  std::string fs =
    #include "shaders/bs_sphere.fs"
      ;

  qpp::cad::shader_program_t *sp = new qpp::cad::shader_program_t(
                                     std::string("bs_sphere"), vs, fs);
  sp->u_on(sp_u_name::m_model_view);
  sp->u_on(sp_u_name::m_proj);
  sp->u_on(sp_u_name::v_color);
  sp->u_on(sp_u_name::f_scale);
  sp->u_on(sp_u_name::f_specular_intensity);
  sp->u_on(sp_u_name::f_specular_alpha);
 // sp->u_on(sp_u_name::f_specular_intensity);
 // sp->u_on(sp_u_name::f_specular_alpha);
  //sp->u_on(sp_u_name::m_model_view_inv_tr);
  //sp->u_on(sp_u_name::v_light_pos);
  //sp->u_on(sp_u_name::v_color);

  return sp;
}

shader_program_t *shader_generators::gen_buf_bs_sphere_program() {
  std::string vs =
    #include "shaders/bs_sphere_buffered.vs"
      ;

  std::string fs =
    #include "shaders/bs_sphere_buffered.fs"
      ;

  shader_program_t *sp = new qpp::cad::shader_program_t(std::string("bs_sphere"), vs, fs);

  sp->u_on(sp_u_name::m_view);
  sp->u_on(sp_u_name::m_proj);
  sp->u_on(sp_u_name::f_scale);
  sp->u_on(sp_u_name::f_specular_intensity);
  sp->u_on(sp_u_name::f_specular_alpha);
  sp->u_on(sp_u_name::texture_0);
  sp->u_on(sp_u_name::texture_1);
 // sp->u_on(sp_u_name::f_specular_intensity);
 // sp->u_on(sp_u_name::f_specular_alpha);
  //sp->u_on(sp_u_name::m_model_view_inv_tr);
  //sp->u_on(sp_u_name::v_light_pos);
  //sp->u_on(sp_u_name::v_color);

  return sp;
}
