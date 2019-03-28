#ifndef QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR
#define QPPCAD_WS_ITEM_EXTENDED_EDITOR_COMPOSITOR

#include <qppcad/qppcad.hpp>
#include <qppcad/ui/qembed_window.hpp>
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

        ws_item_extended_editor_compositor_t(QWidget *parent = nullptr);

      public slots:

        void open_requested();

    };

  }

}

#endif
