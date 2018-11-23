#ifndef QPP_CAD_WS_ITEM_OBJ_INSP_H
#define QPP_CAD_WS_ITEM_OBJ_INSP_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

namespace qpp {

  namespace cad {

    class ws_item_obj_insp_widget_t : public QTabWidget {

        Q_OBJECT

      public:
        QVBoxLayout *main_layout;
        QWidget *tab_general;
        ws_item_obj_insp_widget_t();

    };

  }

}

#endif
