#ifndef QPPCAD_SHADER_PROGRAM_H
#define QPPCAD_SHADER_PROGRAM_H

#include <qppcad/core/qppcad.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

namespace qpp {

namespace cad {

///
/// \brief Uniform fixed name
///
enum sp_u_name {
  m_model_view,
  m_model_view_no_scale,
  m_model_view_inv_tr,
  m_view_inv_tr,
  m_proj,
  m_model_view_proj,
  m_view_proj,
  m_view,
  v_light_pos,
  v_eye_pos,
  f_specular_intensity,
  f_specular_alpha,
  f_norm_mod,
  v_color,
  v_color1,
  v_color2,
  f_color_alpha,
  v_scale,
  v_translate,
  f_scale,
  v_line_start,
  v_line_end,
  texture_0,
  texture_1,
  texture_2,
  texture_3,
  screen_width,
  screen_height
};

///
/// \brief Uniform OpenGL type wrapper
///
enum sp_u_type {
  a_v3f,
  a_v4f,
  a_m3f,
  a_m4f,
  a_sf,
  a_i,
  a_texture
};

/// This structure hold information about internal uniform - is it enabled
/// and it shader uniform variable id
struct uniform_record {
  bool enabled{false};
  GLint h_prog;
};

/// Map uniform internal name to string
static std::map<sp_u_name, std::string> map_u2s = {
  std::make_pair(sp_u_name::m_model_view,          "m_model_view"),
  std::make_pair(sp_u_name::m_model_view_no_scale, "m_model_view_no_scale"),
  std::make_pair(sp_u_name::m_model_view_inv_tr,   "m_model_view_inv_tr"),
  std::make_pair(sp_u_name::m_view_inv_tr,         "m_view_inv_tr"),
  std::make_pair(sp_u_name::m_proj,                "m_proj"),
  std::make_pair(sp_u_name::m_model_view_proj,     "m_model_view_proj"),
  std::make_pair(sp_u_name::m_view_proj,           "m_view_proj"),
  std::make_pair(sp_u_name::m_view,                "m_view"),
  std::make_pair(sp_u_name::v_light_pos,           "v_light_pos"),
  std::make_pair(sp_u_name::v_eye_pos,             "v_eye_pos"),
  std::make_pair(sp_u_name::f_specular_intensity,  "f_specular_intensity"),
  std::make_pair(sp_u_name::f_specular_alpha,      "f_specular_alpha"),
  std::make_pair(sp_u_name::f_norm_mod,            "s_norm_mod"),
  std::make_pair(sp_u_name::v_color,               "v_color"),
  std::make_pair(sp_u_name::v_color1,              "v_color1"),
  std::make_pair(sp_u_name::v_color2,              "v_color2"),
  std::make_pair(sp_u_name::f_color_alpha,         "f_color_alpha"),
  std::make_pair(sp_u_name::v_scale,               "v_scale"),
  std::make_pair(sp_u_name::v_translate,           "v_translate"),
  std::make_pair(sp_u_name::f_scale,               "f_scale"),
  std::make_pair(sp_u_name::v_line_start,          "v_line_start"),
  std::make_pair(sp_u_name::v_line_end,            "v_line_end"),
  std::make_pair(sp_u_name::texture_0,             "texture_0"),
  std::make_pair(sp_u_name::texture_1,             "texture_1"),
  std::make_pair(sp_u_name::texture_2,             "texture_2"),
  std::make_pair(sp_u_name::texture_3,             "texture_3"),
  std::make_pair(sp_u_name::screen_width,          "screen_width"),
  std::make_pair(sp_u_name::screen_height,         "screen_height")
};

/// Map uniform internal name to OpenGL uniform type
static std::map<sp_u_name, sp_u_type> map_u2at = {
  std::make_pair(sp_u_name::m_model_view,          sp_u_type::a_m4f),
  std::make_pair(sp_u_name::m_model_view_no_scale, sp_u_type::a_m4f),
  std::make_pair(sp_u_name::m_model_view_inv_tr,   sp_u_type::a_m4f),
  std::make_pair(sp_u_name::m_view_inv_tr,         sp_u_type::a_m3f),
  std::make_pair(sp_u_name::m_proj,                sp_u_type::a_m4f),
  std::make_pair(sp_u_name::m_model_view_proj,     sp_u_type::a_m4f),
  std::make_pair(sp_u_name::m_view_proj,           sp_u_type::a_m4f),
  std::make_pair(sp_u_name::m_view,                sp_u_type::a_m4f),
  std::make_pair(sp_u_name::v_light_pos,           sp_u_type::a_v3f),
  std::make_pair(sp_u_name::v_eye_pos,             sp_u_type::a_v3f),
  std::make_pair(sp_u_name::f_specular_intensity,  sp_u_type::a_sf),
  std::make_pair(sp_u_name::f_specular_alpha,      sp_u_type::a_sf),
  std::make_pair(sp_u_name::f_norm_mod,            sp_u_type::a_sf),
  std::make_pair(sp_u_name::v_color,               sp_u_type::a_v3f),
  std::make_pair(sp_u_name::v_color1,              sp_u_type::a_v3f),
  std::make_pair(sp_u_name::v_color2,              sp_u_type::a_v3f),
  std::make_pair(sp_u_name::f_color_alpha,         sp_u_type::a_sf),
  std::make_pair(sp_u_name::v_scale,               sp_u_type::a_v3f),
  std::make_pair(sp_u_name::v_translate,           sp_u_type::a_v3f),
  std::make_pair(sp_u_name::f_scale,               sp_u_type::a_sf),
  std::make_pair(sp_u_name::v_line_start,          sp_u_type::a_v3f),
  std::make_pair(sp_u_name::v_line_end,            sp_u_type::a_v3f),
  std::make_pair(sp_u_name::texture_0,             sp_u_type::a_texture),
  std::make_pair(sp_u_name::texture_1,             sp_u_type::a_texture),
  std::make_pair(sp_u_name::texture_2,             sp_u_type::a_texture),
  std::make_pair(sp_u_name::texture_3,             sp_u_type::a_texture),
  std::make_pair(sp_u_name::screen_width,          sp_u_type::a_i),
  std::make_pair(sp_u_name::screen_height,         sp_u_type::a_i),
};

///
/// \brief Shader program implements wrapper around OpenGL program, i.e contains
///  vertex plus fragment shader and some kind of uniform variables management
///
class shader_program_t {

private:

  GLuint program_id;

public:

  std::vector<uniform_record> unf_rec;
  std::string program_name;

  /// \brief shader_program_t
  /// \param _program_name
  /// \param _vs_text
  /// \param _fs_text
  shader_program_t (const std::string &_program_name,
                    const std::string &_vs_text,
                    const std::string &_fs_text);

  /// \brief Enable internal uniform
  /// \param _val
  void u_on (sp_u_name _val);


  /// \brief Set internal uniform
  /// \param _ut
  /// \param _val
  void set_u (sp_u_name _ut, GLfloat *_val);
  void set_u_sampler (sp_u_name _ut, GLint val);

  /// \brief begin_shader_program
  void begin_shader_program ();

  /// \brief end_shader_program
  void end_shader_program ();
};

} // namespace qpp::cad

} // namespace qpp

#endif


