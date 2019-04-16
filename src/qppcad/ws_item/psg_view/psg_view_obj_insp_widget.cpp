#include <qppcad/ws_item/psg_view/psg_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

psg_view_obj_insp_widget_t::psg_view_obj_insp_widget_t() {

  tg_info_sym_gr = new QLabel;
  tg_info_total_sym_op = new QLabel;
  tg_num_axes = new QLabel;
  tg_num_planes = new QLabel;
  tg_bounded_info = new QLabel;
  tg_has_inverse = new QLabel;
  disp_vs_plane_alpha_enabled = new qbinded_checkbox_t;
  disp_vs_show_axes = new qbinded_checkbox_t;
  disp_vs_show_planes = new qbinded_checkbox_t;

  disp_vs_plane_scale = new qbinded_float_spinbox_t;
  disp_vs_plane_scale->set_min_max_step(1, 20, 0.1, 2);
  disp_vs_plane_scale->m_updated_externally_event = true;

  disp_vs_arrow_len = new qbinded_float_spinbox_t;
  disp_vs_arrow_len->set_min_max_step(0.01, 20, 0.05);
  disp_vs_arrow_len->m_updated_externally_event = true;

  disp_vs_arrow_scale = new qbinded_float_spinbox_t;
  disp_vs_arrow_scale->set_min_max_step(0.01, 20, 0.05);
  disp_vs_arrow_scale->m_updated_externally_event = true;

  disp_vs_arrow_cap_len = new qbinded_float_spinbox_t;
  disp_vs_arrow_cap_len->set_min_max_step(0.01, 20, 0.05);
  disp_vs_arrow_cap_len->m_updated_externally_event = true;

  disp_vs_arrow_cap_scale = new qbinded_float_spinbox_t;
  disp_vs_arrow_cap_scale->set_min_max_step(0.01, 20, 0.05);
  disp_vs_arrow_cap_scale->m_updated_externally_event = true;

  disp_vs_plane_color = new qbinded_color3_input_t;
  disp_vs_plane_color->m_updated_externally_event = true;

  gb_psg_summary = new qspoiler_widget_t(tr("Symmetry group summary"));
  gb_psg_summary_lt = new QFormLayout;
  gb_psg_summary->add_content_layout(gb_psg_summary_lt);
  gb_psg_summary_lt->addRow(tr("Sym. gr. name"), tg_info_sym_gr);
  gb_psg_summary_lt->addRow(tr("Num. of op."), tg_info_total_sym_op);
  gb_psg_summary_lt->addRow(tr("Num. of axes"), tg_num_axes);
  gb_psg_summary_lt->addRow(tr("Num. of planes"), tg_num_planes);
  gb_psg_summary_lt->addRow(tr("Has inverse?"), tg_has_inverse);
  gb_psg_summary_lt->addRow(tr("Bounded ?"), tg_bounded_info);
  init_form_lt(gb_psg_summary_lt);

  gb_psg_view_settings = new qspoiler_widget_t(tr("View settings"));
  gb_psg_view_settings_lt = new QFormLayout;
  gb_psg_view_settings->add_content_layout(gb_psg_view_settings_lt);
  gb_psg_view_settings_lt->addRow(tr("Show axes"), disp_vs_show_axes);
  gb_psg_view_settings_lt->addRow(tr("Show planes"), disp_vs_show_planes);
  gb_psg_view_settings_lt->addRow(tr("Plane transp."), disp_vs_plane_alpha_enabled);
  gb_psg_view_settings_lt->addRow(tr("Plane scale"), disp_vs_plane_scale);
  gb_psg_view_settings_lt->addRow(tr("Arrow length"), disp_vs_arrow_len);
  gb_psg_view_settings_lt->addRow(tr("Arrow scale"), disp_vs_arrow_scale);
  gb_psg_view_settings_lt->addRow(tr("Cap length"), disp_vs_arrow_cap_len);
  gb_psg_view_settings_lt->addRow(tr("Cap scale"), disp_vs_arrow_cap_scale);
  gb_psg_view_settings_lt->addRow(tr("Plane color"), disp_vs_plane_color);
  init_form_lt(gb_psg_view_settings_lt);

  //tab colorize orders
  tab_display = def_tab("Colorize group elements", "://images/monitor.svg");

  gb_colorize = new qspoiler_widget_t(tr("Axis colors by order"));
  gb_colorize_lt = new QFormLayout;
  gb_colorize->add_content_layout(gb_colorize_lt);
  for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++) {
      gb_colorize_ctrls[i] = new qbinded_color3_input_t;
      gb_colorize_ctrls[i]->m_updated_externally_event = true;
      gb_colorize_lt->addRow(tr("%1 - order").arg(i+1), gb_colorize_ctrls[i]);
    }
  init_form_lt(gb_colorize_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_psg_summary);
  tab_display->tab_inner_widget_lt->addWidget(gb_psg_view_settings);
  tab_display->tab_inner_widget_lt->addWidget(gb_colorize);

  tab_general->tab_inner_widget_lt->addStretch();
  tab_display->tab_inner_widget_lt->addStretch();

}

void psg_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (_binding_item && _binding_item->get_type() == psg_view_t::get_type_static()) {
      b_pg = _binding_item->cast_as<psg_view_t>();

      if (b_pg && b_pg->m_ag) {
          tg_info_sym_gr->setText(QString::fromStdString(b_pg->m_ag->name));
          tg_info_total_sym_op->setText(tr("%1").arg(b_pg->m_ag->size()));
        }

      disp_vs_plane_alpha_enabled->bind_value(&b_pg->m_plane_alpha_enabled, b_pg);
      disp_vs_plane_scale->bind_value(&b_pg->m_plane_scale, b_pg);
      disp_vs_arrow_len->bind_value(&b_pg->m_arrow_len, b_pg);
      disp_vs_arrow_scale->bind_value(&b_pg->m_arrow_scale, b_pg);
      disp_vs_arrow_cap_len->bind_value(&b_pg->m_arrow_cap_len, b_pg);
      disp_vs_arrow_cap_scale->bind_value(&b_pg->m_arrow_cap_scale, b_pg);
      disp_vs_show_axes->bind_value(&b_pg->m_show_axes);
      disp_vs_show_planes->bind_value(&b_pg->m_show_planes);
      disp_vs_plane_color->bind_value(&b_pg->m_plane_color, b_pg);

      for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++)
          gb_colorize_ctrls[i]->bind_value(&b_pg->m_axes_color_by_order[i], b_pg);

    } else {
      b_pg = nullptr;
    }

  tab_display->tab_inner_widget->update();

  update_from_ws_item();

}

void psg_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_pg) {
      for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++) gb_colorize_ctrls[i]->load_value_ex();
      tg_bounded_info->setText(b_pg->is_bounded() ? tr("Yes") : tr("No"));
      tg_has_inverse->setText(b_pg->m_pg_axes.inversion ? tr("Yes") : tr("No"));
      tg_num_axes->setText(QString("%1").arg(b_pg->m_pg_axes.axes.size()));
      tg_num_planes->setText(QString("%1").arg(b_pg->m_pg_axes.planes.size()));
    }

}

void psg_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  disp_vs_plane_alpha_enabled->unbind_value();
  disp_vs_plane_scale->unbind_value();
  disp_vs_arrow_len->unbind_value();
  disp_vs_arrow_scale->unbind_value();
  disp_vs_arrow_cap_len->unbind_value();
  disp_vs_arrow_cap_scale->unbind_value();
  disp_vs_show_axes->unbind_value();
  disp_vs_show_planes->unbind_value();
  disp_vs_plane_color->unbind_value();

  for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++)
      gb_colorize_ctrls[i]->unbind_value();

  b_pg = nullptr;

}

