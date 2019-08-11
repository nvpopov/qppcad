#ifndef QPPCAD_MESH_H
#define QPPCAD_MESH_H

#include <qppcad/core/qppcad.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <geom/lace3d.hpp>

namespace qpp {

  namespace cad {

    /// \brief The mesh_rt enum
    enum mesh_rt {
      mesh_rt_triangles,
      mesh_rt_lines
    };

    /// \brief The mesh_t class
    class mesh_t {

      public:

        /// Raw vertex data
        std::vector<float> vertecies;
        std::vector<int>   indices;
        std::vector<float> normals;

        uint32_t num_primitives{0};
        uint32_t num_indices{0};

        /// OpenGL buffer handles, todo: move it to template driven abonimation
        GLuint vio;
        GLuint vbo;
        GLuint vao;
        GLuint nbo;
        GLenum mesh_rt;

        mesh_t();

        void construct_explicit();
        /// \brief render
        void render();

        void begin_render_batch();
        void render_batch();
        void end_render_batch();

        /// \brief bind_data
        void bind_data();

    };

    template<typename REAL = float>
    void dump_vector3_to_vector(std::vector<REAL> &invec, const vector3<REAL> _v){
      for (uint8_t i = 0; i < 3; i++) invec.push_back(_v(i));
    }

  } // namespace qpp::cad

} // namespace qpp

#endif
