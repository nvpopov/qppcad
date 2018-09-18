#ifndef QPPCAD_SHADER_GENERATORS
#define QPPCAD_SHADER_GENERATORS

#include <qppcad/qppcad.hpp>
#include <qppcad/shader_program.hpp>

namespace qpp {
  class shader_generators {
    public:
      /// Default shader testing program - Gourand shading
      static shader_program_t* gen_default_program(){
        std::string vs =
    #include "shaders/screen_space_lighting.vs"
            ;

        std::string fs =
    #include "shaders/screen_space_lighting.fs"
            ;

        qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("default_program"), vs, fs);
        sp->u_on(sp_u_name::m_model_view_proj);
        sp->u_on(sp_u_name::m_model_view);
        sp->u_on(sp_u_name::m_model_view_inv_tr);
        sp->u_on(sp_u_name::v_light_pos);
        sp->u_on(sp_u_name::v_translate);
        sp->u_on(sp_u_name::f_scale);
        sp->u_on(sp_u_name::v_color);
        return sp;
      }

      ///
      /// \brief gen_unit_line_program
      /// \return
      ///
      static shader_program_t* gen_unit_line_program(){
        std::string vs =
    #include "shaders/unit_line.vs"
            ;

        std::string fs =
    #include "shaders/unit_line.fs"
            ;

        qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("unit_line_program"), vs, fs);
        sp->u_on(sp_u_name::m_model_view_proj);
        sp->u_on(sp_u_name::m_view);
        sp->u_on(sp_u_name::m_view_inv_tr);
        sp->u_on(sp_u_name::v_color);
        sp->u_on(sp_u_name::v_line_start);
        sp->u_on(sp_u_name::v_line_end);
        return sp;
      }

      ///
      /// \brief gen_line_mesh_program
      /// \return
      ///
      static shader_program_t* gen_line_mesh_program(){
        std::string vs =
    #include "shaders/line_mesh.vs"
            ;

        std::string fs =
    #include "shaders/line_mesh.fs"
            ;

        qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("grid_program"), vs, fs);
        sp->u_on(sp_u_name::m_model_view_proj);
        sp->u_on(sp_u_name::m_model_view);
        //  sp->u_on(sp_u_name::vLightPos);
        sp->u_on(sp_u_name::m_view_inv_tr);
        sp->u_on(sp_u_name::v_translate);
        //  sp->u_on(sp_u_name::fScale);
        sp->u_on(sp_u_name::v_color);
        return sp;
      }

      ///
      /// \brief gen_mv_screen_space_lighting_program
      /// \return
      ///
      static shader_program_t *gen_mv_screen_space_lighting_program(){
        std::string vs =
    #include "shaders/mv_screen_space_lighting.vs"
            ;

        std::string fs =
    #include "shaders/mv_screen_space_lighting.fs"
            ;

        qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("mv_ssl"), vs, fs);
        sp->u_on(sp_u_name::m_model_view_proj);
        sp->u_on(sp_u_name::m_model_view);
        sp->u_on(sp_u_name::m_view_proj);
        sp->u_on(sp_u_name::m_model_view_inv_tr);
        sp->u_on(sp_u_name::v_light_pos);
        sp->u_on(sp_u_name::v_color);
        return sp;
      }

      static  shader_program_t *gen_fbo_quad_program(){
        std::string vs =
    #include "shaders/fb_quad.vs"
            ;

        std::string fs =
    #include "shaders/fb_quad.fs"
            ;
        qpp::shader_program_t *sp = new qpp::shader_program_t(std::string("fbo_quad"), vs, fs);
        sp->u_on(sp_u_name::texture_0);
        return sp;
      }
  };
}

#endif
