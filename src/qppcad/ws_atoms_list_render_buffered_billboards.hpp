#ifndef QPPCAD_BUFFERED_BILLBOARDS
#define QPPCAD_BUFFERED_BILLBOARDS
#include <qppcad/app.hpp>
#include <qppcad/texture_buffer.hpp>

namespace qpp {

  namespace cad {

    template <typename DATA, typename REAL = float, typename AINT  = uint32_t>
    class ws_atoms_list_render_buffered_billboards_t {

      private:

        DATA* p_owner;
        std::unique_ptr<texture_buffer_t<REAL> > m_tb_pos;
        std::unique_ptr<texture_buffer_t<REAL> > m_tb_color_s;

      public:

        ws_atoms_list_render_buffered_billboards_t (DATA &_p_owner) {
          p_owner = &_p_owner;
          init();
        }

        void init();
        void render();
    };

  }

}

#endif
