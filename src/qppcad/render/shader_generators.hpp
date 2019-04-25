#ifndef QPPCAD_SHADER_GENERATORS
#define QPPCAD_SHADER_GENERATORS

#include <qppcad/qppcad.hpp>
#include <qppcad/render/shader_program.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace qpp {

  namespace cad {

    class shader_generators {

      public:

        /// Default shader testing program - Gourand shading
        static shader_program_t* gen_sp_default();

        static shader_program_t* gen_sp_default_suprematic();

        /// \brief gen_unit_line_program
        static shader_program_t* gen_sp_unit_line();

        static shader_program_t* gen_sp_unit_line_styled();

        /// \brief gen_line_mesh_program
        static shader_program_t* gen_sp_line_mesh();

        /// \brief gen_mv_screen_space_lighting_program
        static shader_program_t *gen_sp_mv_screen_space_lighting();

        static shader_program_t *gen_sp_mva_screen_space_lighting();

        static shader_program_t *gen_sp_fbo_quad();

        static shader_program_t *gen_sp_bs_sphere();

        static shader_program_t *gen_sp_buf_bs_sphere();

        static shader_program_t *gen_sp_2c_cylinder();

        static shader_program_t *gen_sp_2c_cylinder_suprematic();
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
