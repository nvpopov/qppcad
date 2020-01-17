#ifndef QPPCAD_GEOM_VIEW_EXTENDED_EDITOR
#define QPPCAD_GEOM_VIEW_EXTENDED_EDITOR

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

        QLabel *m_ext_editor_gv_lbl;
        QVBoxLayout *m_main_lt;
        QTableView *m_xgeom_tv;

        xgeom_table_model_t *m_xgeom_tmdl;
        geom_view_t *m_binded_gv{nullptr};

        geom_view_extended_editor_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
        bool can_be_binded_to(ws_item_t *item) override ;

        QString header_name_hint() override ;

      public slots:

        void selection_changed();

      protected:

        void resizeEvent(QResizeEvent *event) override;
        void header_vertical_double_clicked(int logical_index);
        void header_horizontal_clicked(int logical_index);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
