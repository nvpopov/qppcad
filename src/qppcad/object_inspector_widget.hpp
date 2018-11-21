#ifndef QPP_CAD_OBJECT_INSPECTOR_H
#define QPP_CAD_OBJECT_INSPECTOR_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QHBoxLayout>

namespace qpp {

  namespace cad {

    class object_inspector_widget_t : public QWidget {
      public:
        object_inspector_widget_t();
        QGroupBox *ws_items_gb;
        QHBoxLayout *ws_items_gb_layout;
        QListWidget *ws_items_list;
        QHBoxLayout *obj_insp_layout;
    };

  }

}


#endif
