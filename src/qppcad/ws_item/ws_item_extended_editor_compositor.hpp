#ifndef QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR
#define QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR

#include <qppcad/qppcad.hpp>
#include <qppcad/ui/qembed_window.hpp>
#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class ws_item_extended_editor_compositor_t : public qembed_window_t {

        Q_OBJECT

      public:

        QWidget *ext_edt_plch;
        QPushButton *top_btn_close;
        QPushButton *top_btn_refresh;

        std::shared_ptr<ws_item_extended_editor_t> m_cur_ext_editor_widget{nullptr};

        ws_item_extended_editor_compositor_t(QWidget *parent = nullptr);
        ~ws_item_extended_editor_compositor_t();

        void open_extended_editor(size_t editor_id = 0);

      public slots:

        void open_requested();
        void open_requested_with_order(size_t editor_id);
        void cur_ws_selected_item_changed();
        void cur_ws_changed();

        void close_ext_editor_btn_clicked();
        void refresh_ext_editor_btn_clicked();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
