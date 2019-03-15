#ifndef QPPCAD_SIZE_GUIDE
#define QPPCAD_SIZE_GUIDE

#include <qppcad/qppcad.hpp>
#include <QScreen>

namespace qpp {

  namespace cad {

    class size_guide_t {

      public:

        size_guide_t();

        int main_window_h();
        int main_window_w();

        int tool_panel_h();
        int tool_panel_ws_selector_w();
        int tool_panel_ws_selector_h();
        int tool_panel_ctrl_h();
        int tool_panel_ctrl_w();
        int tool_panel_edit_sel_w();
        int tool_panel_ctrl_sys_w();
        int tool_panel_checkable_ctrl_w();
        int tool_panel_icon_size();

        int obj_insp_w();
        int obj_insp_table_w();
        int obj_insp_item_list_max_h();
        int obj_insp_ctrl_max_w();
        int obj_insp_combo_max_w();
        int obj_insp_ctrl_t2_w();
        int obj_insp_ctrl_t2_h();

    };

  }

}


#endif
