#ifndef QPPCAD_WS_TRAJ_HIGHLIGHT_OBJECT_INSP
#define QPPCAD_WS_TRAJ_HIGHLIGHT_OBJECT_INSP
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/traj_hl/traj_hl.hpp>

namespace qpp {

  namespace cad {

    class traj_hl_obj_insp_widget_t final: public ws_item_obj_insp_widget_t {

        Q_OBJECT

      public:
        qspoiler_widget_t *gb_traj_settings;
        QFormLayout *gb_traj_settings_lt;
        traj_hl_t *b_th{nullptr};

        qbinded_color3_input_t *disp_traj_color;

        traj_hl_obj_insp_widget_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
