#ifndef QPPCAD_SHADER_GENERATORS
#define QPPCAD_SHADER_GENERATORS

#include <qppcad/qppcad.hpp>
#include <qppcad/shader_program.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace qpp {

  namespace cad {

    class shader_generators {

      public:

        /// Default shader testing program - Gourand shading
        static shader_program_t* gen_default_program(QOpenGLContext *context);

        /// \brief gen_unit_line_program
        static shader_program_t* gen_unit_line_program(QOpenGLContext *context);

        static shader_program_t* gen_unit_line_styled_program(QOpenGLContext *context);

        /// \brief gen_line_mesh_program
        static shader_program_t* gen_line_mesh_program(QOpenGLContext *context);

        /// \brief gen_mv_screen_space_lighting_program
        static shader_program_t *gen_mv_screen_space_lighting_program(QOpenGLContext *context);

        static shader_program_t *gen_fbo_quad_program(QOpenGLContext *context);

        static shader_program_t *gen_bs_sphere_program(QOpenGLContext *context);

        static shader_program_t *gen_buf_bs_sphere_program(QOpenGLContext *context);
    };

  }

}

#endif
