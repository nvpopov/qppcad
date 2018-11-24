#ifndef QPP_CAD_WS_ITEM_OBJ_INSP_H
#define QPP_CAD_WS_ITEM_OBJ_INSP_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>

namespace qpp {

  namespace cad {

    class ws_item_obj_insp_widget_t : public QTabWidget {

        Q_OBJECT

      public:
        QVBoxLayout *main_layout;
        QWidget *tab_general;
        QScrollArea *tab_general_scroll_area;
        QVBoxLayout *tab_general_layout;

        ws_item_obj_insp_widget_t();

    };

  }

}

#endif
