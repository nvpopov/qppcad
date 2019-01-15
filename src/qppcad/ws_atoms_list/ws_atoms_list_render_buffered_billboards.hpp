#ifndef QPPCAD_BUFFERED_BILLBOARDS
#define QPPCAD_BUFFERED_BILLBOARDS
//#include <qppcad/app.hpp>
#include <qppcad/texture_buffer.hpp>

namespace qpp {

  namespace cad {

    class ws_atoms_list_t;

    class ws_atoms_list_render_buffered_billboards_t {

      private:

        ws_atoms_list_t* p_owner;
//        std::unique_ptr<texture_buffer_t> m_tb_pos{};
//        std::unique_ptr<texture_buffer_t> m_tb_color_s{};

      public:

        ws_atoms_list_render_buffered_billboards_t (ws_atoms_list_t &_p_owner);
        void init();
        void render();
    };

  }

}

#endif
