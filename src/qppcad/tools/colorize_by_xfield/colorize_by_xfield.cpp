#include <qppcad/tools/colorize_by_xfield/colorize_by_xfield.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools_colorize.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qt_helpers.hpp>

using namespace qpp;
using namespace qpp::cad;

void colorize_by_xfield_tool_t::exec(ws_item_t *item, uint32_t _error_ctx) {

  app_state_t *astate = app_state_t::get_inst();

  if (!item) {
      return;
    }

  auto gv_ptr = item->cast_as<geom_view_t>();

  if (!gv_ptr) {
      return;
    }

  colorize_by_xfield_widget_t cw;
  cw.b_gv = gv_ptr;
  cw.build_xfield();
  int res = cw.exec();

  if (res == QDialog::Accepted) {

      std::string xfield_name = cw.m_cmb_xfield_name->currentText().toStdString();
      auto xfield_it = std::find(cw.b_fn.begin(), cw.b_fn.end(), xfield_name);
      auto xfield_idx = std::distance(cw.b_fn.begin(), xfield_it);

      if (xfield_idx >=0 && xfield_idx < cw.b_fn.size()) {
          //std::cout << "COLORIZE_BY_XFIELD " << xfield_idx << std::endl;
          geom_view_colorizer_helper::colorize_by_xfield(gv_ptr,
                                                         cw.m_clr_low.get_value(),
                                                         cw.m_clr_high.get_value(),
                                                         xfield_idx);
          gv_ptr->m_color_mode.set_value(geom_view_color_e::color_from_xgeom);
          astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();
          astate->make_viewport_dirty();

        }

    }

}

void colorize_by_xfield_widget_t::build_xfield() {

  if (!b_gv) {
      return;
    }

  b_gv->m_geom->get_format(b_fn, b_ft);

  for (size_t i = 0; i < b_fn.size(); i++)
    if (b_ft[i] == basic_types::type_real)
      m_cmb_xfield_name->addItem(QString::fromStdString(b_fn[i]));

}

colorize_by_xfield_widget_t::colorize_by_xfield_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  m_main_lt = new QVBoxLayout;
  setLayout(m_main_lt);
  setFixedWidth(astate->size_guide.ws_item_modal_tool_small_width());

  m_main_gb = new qspoiler_widget_t(tr("Colorize by xfield : parameters"), nullptr, false, 6,
                                    astate->size_guide.ws_item_modal_tool_small_gb_width());
  m_main_gb->set_top_border(false);
  m_main_gb_lt = new QFormLayout;
  m_main_gb->add_content_layout(m_main_gb_lt);

  m_clr_low_input = new qbinded_color3_input_t;
  m_clr_low_input->bind_value(&m_clr_low);
  m_clr_high_input = new qbinded_color3_input_t;
  m_clr_high_input->bind_value(&m_clr_high);

  m_cmb_xfield_name = new QComboBox;
  m_cmb_xfield_name->setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());

  m_main_gb_lt->addRow(tr("XField name"), m_cmb_xfield_name);
  m_main_gb_lt->addRow(tr("Color at min"), m_clr_low_input);
  m_main_gb_lt->addRow(tr("Color at max"), m_clr_high_input);
  qt_hlp::resize_form_lt_lbls(m_main_gb_lt,
                              astate->size_guide.ws_item_modal_tool_small_form_label_width());

  m_buttons_lt = new QHBoxLayout;
  m_button_apply = new QPushButton(tr("Apply"));
  m_button_cancel = new QPushButton((tr("Cancel")));
  m_buttons_lt->addWidget(m_button_apply);
  m_buttons_lt->addWidget(m_button_cancel);

  m_main_lt->addWidget(m_main_gb);
  m_main_lt->addLayout(m_buttons_lt);

  connect(m_button_apply,
          &QPushButton::clicked,
          this,
          &colorize_by_xfield_widget_t::accept);

  connect(m_button_cancel,
          &QPushButton::clicked,
          this,
          &colorize_by_xfield_widget_t::reject);

}
