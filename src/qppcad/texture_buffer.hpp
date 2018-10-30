#ifndef QPPCAD_TEXTURE_BUFFER
#define QPPCAD_TEXTURE_BUFFER
#include <qppcad/app.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace qpp {

  namespace cad {

    template<typename REAL, int COLS>
    using row_dynamic_matrix_t = Eigen::Matrix<REAL, Eigen::Dynamic, COLS, Eigen::ColMajor>;

    class texture_buffer_t {
      public:
        GLenum m_tx_id;
        GLenum m_tx_buffer_id;
        row_dynamic_matrix_t<float,4> m_data;
        void init();
        void bind(const int slot);
        void unbind(const int slot);
        void finalize();
    };

  }

}

#endif
