#include <qppcad/ws_item/arrow_array/arrow_array_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

arrow_array_obj_insp_widget_t::arrow_array_obj_insp_widget_t() {

  app_state_t *astate = app_state_t::get_inst();

  gb_aa_settings = new qspoiler_widget_t(tr("Arrow Array Settings"));
  gb_aa_settings_lt = new QFormLayout;
  gb_aa_settings->add_content_layout(gb_aa_settings_lt);

  aa_settings_src = new qbinded_ws_item_combobox_t;
  aa_settings_src->m_updated_externally_event = true;
  aa_settings_src->setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());
  aa_settings_src->m_type_id = geom_view_t::get_type_static();

  aa_unf_arrow_len = new qbinded_float_spinbox_t;
  aa_unf_arrow_len->set_min_max_step(0.01, 20, 0.01);

  aa_unf_arrow_scale = new qbinded_float_spinbox_t;
  aa_unf_arrow_scale->set_min_max_step(0.01, 20, 0.01);

  aa_unf_arrow_cap_len = new qbinded_float_spinbox_t;
  aa_unf_arrow_cap_len->set_min_max_step(0.01, 20, 0.01);

  aa_unf_arrow_cap_scale = new qbinded_float_spinbox_t;
  aa_unf_arrow_cap_scale->set_min_max_step(0.01, 20, 0.01);

  aa_displ_eps = new qbinded_float_spinbox_t;
  aa_displ_eps->set_min_max_step(0.0001f, 1, 0.0001f);
  aa_displ_eps->setDecimals(4);

  aa_arrow_color = new qbinded_color3_input_t;

  aa_affected_by_sv = new qbinded_checkbox_t;

  gb_aa_settings_lt->addRow(tr("Source"), aa_settings_src);
  gb_aa_settings_lt->addRow(tr("Displ. eps."), aa_displ_eps);
  gb_aa_settings_lt->addRow(tr("Arrow length"), aa_unf_arrow_len);
  gb_aa_settings_lt->addRow(tr("Arrow scale"), aa_unf_arrow_scale);
  gb_aa_settings_lt->addRow(tr("Cap length"), aa_unf_arrow_cap_len);
  gb_aa_settings_lt->addRow(tr("Cap scale"), aa_unf_arrow_cap_scale);
  gb_aa_settings_lt->addRow(tr("Arrow color"), aa_arrow_color);
  gb_aa_settings_lt->addRow(tr("Affected by sel. vis."), aa_affected_by_sv);

  init_form_lt(gb_aa_settings_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_aa_settings);
  tab_general->tab_inner_widget_lt->addStretch();

}

void arrow_array_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (auto _as_aa = _binding_item->cast_as<arrow_array_t>(); _as_aa != nullptr) {

      b_aa = _as_aa;
      aa_settings_src->bind_value(&b_aa->m_src, _as_aa, _as_aa->m_parent_ws);
      aa_displ_eps->bind_value(&b_aa->m_displ_eps, _as_aa);
      aa_unf_arrow_len->bind_value(&b_aa->m_unf_arrow_len);
      aa_unf_arrow_scale->bind_value(&b_aa->m_unf_arrow_scale);
      aa_unf_arrow_cap_len->bind_value(&b_aa->m_unf_arrow_cap_len);
      aa_unf_arrow_cap_scale->bind_value(&b_aa->m_unf_arrow_cap_scale);
      aa_arrow_color->bind_value(&b_aa->m_color);
      aa_affected_by_sv->bind_value(&b_aa->m_affected_by_sv);

    }

}

void arrow_array_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void arrow_array_obj_insp_widget_t::unbind_item() {

  aa_settings_src->unbind_value();
  aa_displ_eps->unbind_value();
  aa_unf_arrow_len->unbind_value();
  aa_unf_arrow_scale->unbind_value();
  aa_unf_arrow_cap_len->unbind_value();
  aa_unf_arrow_cap_scale->unbind_value();
  aa_arrow_color->unbind_value();
  aa_affected_by_sv->unbind_value();

}
