#ifndef QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR
#define QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class ws_item_extended_editor_compositor_t : public QFrame {

        Q_OBJECT

      public:

        QVBoxLayout *main_lt;
        QHBoxLayout *top_btn_lt;

        QPushButton *top_btn_close;

        QWidget *ext_edt_plch;

        ws_item_extended_editor_compositor_t();

      public slots:

        void open_requested();

    };

  }

}

#endif
