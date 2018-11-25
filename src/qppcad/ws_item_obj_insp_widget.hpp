#ifndef QPP_CAD_WS_ITEM_OBJ_INSP_H
#define QPP_CAD_WS_ITEM_OBJ_INSP_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <qppcad/ws_item.hpp>
#include <QGroupBox>
#include <QCheckBox>
#include <qppcad/qbinded_inputs.hpp>

namespace qpp {

  namespace cad {

    struct ws_item_tab_widget_t {
        QScrollArea *tab_scroll;
        QWidget *tab_inner_widget;
        QVBoxLayout *tab_inner_widget_layout;
    };

    class ws_item_obj_insp_widget_t : public QTabWidget {

        Q_OBJECT

      public:
        ws_item_t *m_binded_item;

        ws_item_tab_widget_t *tab_general;
        QGroupBox *tg_info_widget;
        QFormLayout *tg_form_layout;
        QLabel *ws_item_name;
        QLabel *ws_item_type;

        QLabel *ws_item_is_visible_label;
        qbinded_checkbox *ws_item_is_visible;

        ws_item_tab_widget_t* define_tab(QString tab_name);
        virtual void bind_to_item(ws_item_t *_binding_item);
        void unbind_item();
        virtual void update_from_ws_item();
        ws_item_obj_insp_widget_t();

    };

  }

}

#endif
