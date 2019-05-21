#ifndef QPP_CAD_PDOS_VIEW_EXTENDED_EDITOR
#define QPP_CAD_PDOS_VIEW_EXTENDED_EDITOR
#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace qpp {

  namespace cad {

    class pdos_view_extended_editor_t : public ws_item_extended_editor_t {

        Q_OBJECT

      public:

        pdos_view_t *m_binded_nx{nullptr};
        QVBoxLayout *main_lt;

        // general pdos plot
        QChartView *pdos_gen_chart_view;
        // end of general pdos plot

        pdos_view_extended_editor_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        bool can_be_binded_to(ws_item_t *item) override;

        QString header_name_hint() override;

    };

  } // namespace cad

} // namespace qpp

#endif
