#ifndef QPPCAD_OBJECT_INSPECTOR_H
#define QPPCAD_OBJECT_INSPECTOR_H

#include <qppcad/qppcad.hpp>
#include <QGroupBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ui/qembed_window.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>

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
        qspoiler_widget_t *ws_items_spoiler;
        QVBoxLayout *ws_items_spoiler_lt;

        QPushButton *btn_add_new_ws_item;
        QPushButton *btn_refresh_oi;

        //property view by ws_item type
        QWidget *none_item_placeholder;
        //QWidget *ws_current_view{nullptr};
        std::shared_ptr<ws_item_obj_insp_widget_t> m_cur_obj_insp_widget{nullptr};

      public slots:

        void cur_ws_changed();
        void cur_ws_selected_item_changed();
        void ui_cur_ws_selected_item_changed();
        void need_to_update_obj_insp_received();

        void ws_item_list_double_clicked(QListWidgetItem * item);
        void refresh_button_clicked();
        void add_new_ws_item_button_clicked();

        void provide_context_menu_for_ws_items(const QPoint &pos);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
