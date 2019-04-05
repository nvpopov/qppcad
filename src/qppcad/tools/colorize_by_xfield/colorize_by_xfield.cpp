
#include <qppcad/tools/colorize_by_xfield/colorize_by_xfield.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void colorize_by_xfield_tool_t::exec(ws_item_t *item) {

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

      std::string xfield_name = cw.cmb_xfield_name->currentText().toStdString();
      auto xfield_it = std::find(cw.b_fn.begin(), cw.b_fn.end(), xfield_name);
      auto xfield_idx = std::distance(cw.b_fn.begin(), xfield_it);

      if (xfield_idx >=0 && xfield_idx < cw.b_fn.size()) {
          std::cout << "COLORIZE_BY_XFIELD " << xfield_idx << std::endl;
          gv_ptr->colorize_by_xfield(cw.clr_low, cw.clr_high, xfield_idx);
          gv_ptr->m_color_mode = geom_view_color_e::color_from_xgeom;
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
    if (b_ft[i] == type_real) cmb_xfield_name->addItem(QString::fromStdString(b_fn[i]));

}

colorize_by_xfield_widget_t::colorize_by_xfield_widget_t() {

  main_lt = new QVBoxLayout;
  setLayout(main_lt);

  main_gb = new QGroupBox(tr("Colorize parameters"));
  main_gb_lt = new QFormLayout;
  main_gb->setLayout(main_gb_lt);

  clr_low_input = new qbinded_color3_input_t;
  clr_low_input->bind_value(&clr_low);
  clr_high_input = new qbinded_color3_input_t;
  clr_high_input->bind_value(&clr_high);

  cmb_xfield_name = new QComboBox;

  main_gb_lt->addRow(tr("XField name :"), cmb_xfield_name);
  main_gb_lt->addRow(tr("Color at min :"), clr_low_input);
  main_gb_lt->addRow(tr("Color at max :"), clr_high_input);

  buttons_lt = new QHBoxLayout;
  button_apply = new QPushButton(tr("Apply"));
  button_cancel = new QPushButton((tr("Cancel")));
  buttons_lt->addWidget(button_apply);
  buttons_lt->addWidget(button_cancel);

  main_lt->addWidget(main_gb);
  main_lt->addLayout(buttons_lt);

  connect(button_apply,
          &QPushButton::clicked,
          this,
          &colorize_by_xfield_widget_t::accept);

  connect(button_cancel,
          &QPushButton::clicked,
          this,
          &colorize_by_xfield_widget_t::reject);

}
