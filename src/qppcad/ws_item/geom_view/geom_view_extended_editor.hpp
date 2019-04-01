#ifndef QPP_CAD_GEOM_VIEW_EXTENDED_EDITOR
#define QPP_CAD_GEOM_VIEW_EXTENDED_EDITOR

#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/xgeom_table_model.hpp>

#include <QTableView>
#include <QHeaderView>

namespace qpp {

  namespace cad {

    class geom_view_extended_editor_t : public ws_item_extended_editor_t {

        Q_OBJECT

      public:

        QLabel *ext_editor_gv_label;
        QVBoxLayout *main_lt;
        QTableView *xgeom_tv;

        xgeom_table_model_t *xgeom_tmdl;
        geom_view_t *m_binded_gv{nullptr};

        geom_view_extended_editor_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

      public slots:

        void selection_changed();

      protected:

        void resizeEvent(QResizeEvent *event) override;
        void data_double_clicked(int logical_index);

    };

  }

}

#endif
