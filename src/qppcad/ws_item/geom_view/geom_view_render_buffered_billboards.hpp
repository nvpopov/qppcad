#ifndef QPPCAD_BUFFERED_BILLBOARDS
#define QPPCAD_BUFFERED_BILLBOARDS
//#include <qppcad/app.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class geom_view_render_buffered_billboards_t {

      private:

        geom_view_t* p_owner;
//        std::unique_ptr<texture_buffer_t> m_tb_pos{};
//        std::unique_ptr<texture_buffer_t> m_tb_color_s{};

      public:

        geom_view_render_buffered_billboards_t (geom_view_t &_p_owner);
        void init();
        void render();
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
