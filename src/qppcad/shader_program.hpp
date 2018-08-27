#ifndef QPP_SHADER_PROGRAM_H
#define QPP_SHADER_PROGRAM_H
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <io/strfun.hpp>
#include <tuple>

namespace qpp {

  /// Uniform fixed name
  enum sp_u_name {
    mModelView,
    mModelViewNoScale,
    mModelViewInvTr,
    mViewInvTr,
    mModelViewProj,
    mViewProj,
    mView,
    vLightPos,
    vColor,
    vScale,
    vTranslate,
    fScale,
    vLineStart,
    vLineEnd
  };

  /// Uniform OpenGL type wrapper
  enum sp_u_type {
    a_v3f,
    a_v4f,
    a_m3f,
    a_m4f,
    a_sf
  };

  /// This structure hold information about internal uniform - is it enabled
  /// and it shader uniform variable id
  struct uniform_record {
    bool bEnabled;
    GLint iProgVar;
  };

  /// Map uniform internal name to string
  static std::map<sp_u_name, std::string> map_u2s = {
    std::make_pair(sp_u_name::mModelView,        "mMV"),
    std::make_pair(sp_u_name::mModelViewNoScale, "mMV_NoScale"),
    std::make_pair(sp_u_name::mModelViewInvTr,   "mMV_InvTr"),
    std::make_pair(sp_u_name::mViewInvTr,        "mV_InvTr"),
    std::make_pair(sp_u_name::mModelViewProj,    "mMVP"),
    std::make_pair(sp_u_name::mViewProj,         "mVP"),
    std::make_pair(sp_u_name::mView,             "mV"),
    std::make_pair(sp_u_name::vLightPos,         "vLightPos"),
    std::make_pair(sp_u_name::vColor,            "vColor"),
    std::make_pair(sp_u_name::vScale,            "vScale"),
    std::make_pair(sp_u_name::vTranslate,        "vTranslate"),
    std::make_pair(sp_u_name::fScale,            "fScale"),
    std::make_pair(sp_u_name::vLineStart,        "vLineStart"),
    std::make_pair(sp_u_name::vLineEnd,          "vLineEnd")
  };

  /// Map uniform internal name to OpenGL uniform type
  static std::map<sp_u_name, sp_u_type> map_u2at = {
    std::make_pair(sp_u_name::mModelView,        sp_u_type::a_m4f),
    std::make_pair(sp_u_name::mModelViewNoScale, sp_u_type::a_m4f),
    std::make_pair(sp_u_name::mModelViewInvTr,   sp_u_type::a_m4f),
    std::make_pair(sp_u_name::mViewInvTr,        sp_u_type::a_m3f),
    std::make_pair(sp_u_name::mModelViewProj,    sp_u_type::a_m4f),
    std::make_pair(sp_u_name::mViewProj,         sp_u_type::a_m4f),
    std::make_pair(sp_u_name::mView,             sp_u_type::a_m4f),
    std::make_pair(sp_u_name::vLightPos,         sp_u_type::a_v3f),
    std::make_pair(sp_u_name::vColor,            sp_u_type::a_v3f),
    std::make_pair(sp_u_name::vScale,            sp_u_type::a_v3f),
    std::make_pair(sp_u_name::vTranslate,        sp_u_type::a_v3f),
    std::make_pair(sp_u_name::fScale,            sp_u_type::a_sf),
    std::make_pair(sp_u_name::vLineStart,        sp_u_type::a_v3f),
    std::make_pair(sp_u_name::vLineEnd,          sp_u_type::a_v3f)
  };

  /// Shader program implements wrapper around OpenGL program, i.e contains
  /// vertex plus fragment shader and some kind of uniform variables management
  class shader_program{
  private:
    GLuint programID;
  public:
    std::vector<uniform_record> unfRec;
    std::string programName;
    shader_program(const std::string _programName,
                   const std::string &_vs_text,
                   const std::string &_fs_text);

    /// Enable internal uniform
    void u_on(sp_u_name _val);

    /// Set internal uniform
    void set_u(sp_u_name _ut, GLfloat *_val);
    void begin_shader_program();
    void end_shader_program();
  };

  /// Default shader testing program - Gourand shading
  shader_program* gen_default_program();

  shader_program* gen_unit_line_program();
  shader_program* gen_bond_draw_program();
  shader_program* gen_line_mesh_program();
  shader_program *gen_screen_space_lighting_program();
}
#endif


