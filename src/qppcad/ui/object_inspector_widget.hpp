#ifndef QPP_CAD_OBJECT_INSPECTOR_H
#define QPP_CAD_OBJECT_INSPECTOR_H

#include <qppcad/qppcad.hpp>
#include <QGroupBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ui/qembed_window.hpp>

namespace qpp {

  namespace cad {

    class object_inspector_widget_t : public qembed_window_t {

        Q_OBJECT

      public:

        object_inspector_widget_t(QWidget *parent = 0);
        ~object_inspector_widget_t();

        Q_PROPERTY(QString class READ cssClass)
        QString cssClass() { return QString("object_inspector_widget"); }

        void update_ws_items_view_widget();

        QLabel *ws_items_label;
        QListWidget *ws_items_list;
        //QVBoxLayout *obj_insp_layout;
        //QSpacerItem *obj_insp_layout_spacer;

        //property view by ws_item type
        QWidget *none_item_placeholder;
        //QWidget *ws_current_view{nullptr};
        std::shared_ptr<ws_item_obj_insp_widget_t> m_cur_obj_insp_widget{nullptr};

      private slots:

        void cur_ws_changed();
        void cur_ws_selected_item_changed();
        void ui_cur_ws_selected_item_changed();
        void need_to_update_obj_insp_received();

        void ws_item_list_double_clicked(QListWidgetItem * item);

    };

  }

}


#endif
