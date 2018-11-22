#ifndef QPP_CAD_OBJECT_INSPECTOR_H
#define QPP_CAD_OBJECT_INSPECTOR_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QGroupBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>

namespace qpp {

  namespace cad {

    class object_inspector_widget_t : public QWidget {

        Q_OBJECT

      public:
        object_inspector_widget_t();
        QLabel *ws_items_label;
        QListWidget *ws_items_list;
        QVBoxLayout *obj_insp_layout;

      private slots:
        void current_workspace_changed();
        void current_workspace_selected_item_changed();
        void ui_current_workspace_selected_item_changed();
    };

  }

}


#endif
