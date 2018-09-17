#ifndef QPP_SHADER_PROGRAM_H
#define QPP_SHADER_PROGRAM_H

#include <qppcad/qppcad.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace qpp {

  ///
  /// \brief Uniform fixed name
  ///
  enum sp_u_name {
    m_model_view,
    m_model_view_no_scale,
    m_model_view_inv_tr,
    m_view_inv_tr,
    m_model_view_proj,
    m_view_proj,
    m_view,
    v_light_pos,
    v_color,
    v_scale,
    v_translate,
    f_scale,
    v_line_start,
    v_line_end,
    texture_0,
    texture_1,
    texture_2,
    texture_3
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
    std::make_pair(sp_u_name::m_model_view_proj,     "m_model_view_proj"),
    std::make_pair(sp_u_name::m_view_proj,           "m_view_proj"),
    std::make_pair(sp_u_name::m_view,                "m_view"),
    std::make_pair(sp_u_name::v_light_pos,           "v_light_pos"),
    std::make_pair(sp_u_name::v_color,               "v_color"),
    std::make_pair(sp_u_name::v_scale,               "v_scale"),
    std::make_pair(sp_u_name::v_translate,           "v_translate"),
    std::make_pair(sp_u_name::f_scale,               "f_scale"),
    std::make_pair(sp_u_name::v_line_start,          "v_line_start"),
    std::make_pair(sp_u_name::v_line_end,            "v_line_end"),
    std::make_pair(sp_u_name::texture_0,             "texture_0"),
    std::make_pair(sp_u_name::texture_1,             "texture_1"),
    std::make_pair(sp_u_name::texture_2,             "texture_2"),
    std::make_pair(sp_u_name::texture_3,             "texture_3"),
  };

  /// Map uniform internal name to OpenGL uniform type
  static std::map<sp_u_name, sp_u_type> map_u2at = {
    std::make_pair(sp_u_name::m_model_view,          sp_u_type::a_m4f),
    std::make_pair(sp_u_name::m_model_view_no_scale, sp_u_type::a_m4f),
    std::make_pair(sp_u_name::m_model_view_inv_tr,   sp_u_type::a_m4f),
    std::make_pair(sp_u_name::m_view_inv_tr,         sp_u_type::a_m3f),
    std::make_pair(sp_u_name::m_model_view_proj,     sp_u_type::a_m4f),
    std::make_pair(sp_u_name::m_view_proj,           sp_u_type::a_m4f),
    std::make_pair(sp_u_name::m_view,                sp_u_type::a_m4f),
    std::make_pair(sp_u_name::v_light_pos,           sp_u_type::a_v3f),
    std::make_pair(sp_u_name::v_color,               sp_u_type::a_v3f),
    std::make_pair(sp_u_name::v_scale,               sp_u_type::a_v3f),
    std::make_pair(sp_u_name::v_translate,           sp_u_type::a_v3f),
    std::make_pair(sp_u_name::f_scale,               sp_u_type::a_sf),
    std::make_pair(sp_u_name::v_line_start,          sp_u_type::a_v3f),
    std::make_pair(sp_u_name::v_line_end,            sp_u_type::a_v3f),
    std::make_pair(sp_u_name::texture_0,             sp_u_type::a_texture),
    std::make_pair(sp_u_name::texture_1,             sp_u_type::a_texture),
    std::make_pair(sp_u_name::texture_2,             sp_u_type::a_texture),
    std::make_pair(sp_u_name::texture_3,             sp_u_type::a_texture)
  };

  ///
  /// \brief Shader program implements wrapper around OpenGL program, i.e contains
  ///  vertex plus fragment shader and some kind of uniform variables management
  ///
  class shader_program_t{
    private:
      GLuint program_id;
    public:
      std::vector<uniform_record> unf_rec;
      std::string program_name;

      ///
      /// \brief shader_program_t
      /// \param _program_name
      /// \param _vs_text
      /// \param _fs_text
      ///
      shader_program_t(const std::string _program_name,
                       const std::string &_vs_text,
                       const std::string &_fs_text);

      ///
      /// \brief Enable internal uniform
      /// \param _val
      ///
      void u_on(sp_u_name _val);


      ///
      /// \brief Set internal uniform
      /// \param _ut
      /// \param _val
      ///
      void set_u(sp_u_name _ut, GLfloat *_val);

      ///
      /// \brief begin_shader_program
      ///
      void begin_shader_program();

      ///
      /// \brief end_shader_program
      ///
      void end_shader_program();
  };

  /// Default shader testing program - Gourand shading
  shader_program_t* gen_default_program();

  ///
  /// \brief gen_unit_line_program
  /// \return
  ///
  shader_program_t* gen_unit_line_program();

  ///
  /// \brief gen_line_mesh_program
  /// \return
  ///
  shader_program_t* gen_line_mesh_program();

  ///
  /// \brief gen_screen_space_lighting_program
  /// \return
  ///
  shader_program_t* gen_screen_space_lighting_program();

  ///
  /// \brief gen_mv_screen_space_lighting_program
  /// \return
  ///
  shader_program_t *gen_mv_screen_space_lighting_program();

  shader_program_t *gen_fbo_quad_program();
}
#endif


