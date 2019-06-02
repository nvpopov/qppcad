#ifndef QPPCAD_SIZE_GUIDE
#define QPPCAD_SIZE_GUIDE

#include <qppcad/qppcad.hpp>
#include <QScreen>

namespace qpp {

  namespace cad {

    /**
     * @brief The size_guide_t class implements single end point for controlling sizes of ui
     * elements and fonts
     */
    class size_guide_t {

      public:

        size_guide_t();

        /**
         * @brief main_window_h
         * @return minimum height of app`s main window
         */
        int main_window_h();

        /**
         * @brief main_window_w
         * @return minimum width of app`s main window
         */
        int main_window_w();

        /**
         * @brief tool_panel_h
         * @return fixed height of app`s tool panel
         */
        int tool_panel_h();
        int tool_panel_h_exact();

        /**
         * @brief tool_panel_ws_selector_w
         * @return fixed width of workspace selector tool
         */
        int tool_panel_ws_selector_w();

        /**
         * @brief tool_panel_ws_selector_h
         * @return fixed height of workspace selector tool
         */
        int tool_panel_ws_selector_h();

        int tool_panel_ctrl_h();
        int tool_panel_ctrl_w();
        int tool_panel_edit_sel_w();
        int tool_panel_ctrl_sys_w();
        int tool_panel_checkable_ctrl_w();
        int tool_panel_icon_size();

        int obj_insp_w();
        int obj_insp_table_w();
        int obj_insp_anim_w();
        int obj_insp_item_list_max_h();
        int obj_insp_ctrl_max_w();
        int obj_insp_combo_max_w();
        int obj_insp_combo_max_w_v2();
        int obj_insp_ctrl_t2_w();
        int obj_insp_ctrl_t2_h();
        int obj_insp_button_w();
        int obj_insp_button_h();
        int obj_insp_lbl_w();
        int obj_insp_list_wdgt_h();

        int ext_editor_btn_h();
        int ext_editor_icon();

        int spoiler_button_h();
        int spoiler_button_icon_h();

        /**
         * @brief common_tools_panel_w
         * @return fixed width for various tools left panels
         */
        int common_tools_panel_w();
        int common_tools_panel_label_w();

        int script_editor_button_w();
        int script_edtior_button_icon_w();

        int common_button_fixed_w();

        int node_book_inplace_par_width();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
