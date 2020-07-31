#include <qppcad/ws_item/psg_view/psg_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

psg_view_obj_insp_widget_t::psg_view_obj_insp_widget_t() {

  m_tg_info_sym_gr = new QLabel;
  m_tg_info_numbers = new QLabel;
  m_tg_info_general = new QLabel;

  m_disp_vs_plane_alpha_enabled = new qbinded_checkbox_t;
  m_disp_vs_show_axes = new qbinded_checkbox_t;
  m_disp_vs_show_planes = new qbinded_checkbox_t;

  m_disp_vs_plane_scale = new qbinded_float_spinbox_t;
  m_disp_vs_plane_scale->set_min_max_step(1, 20, 0.1, 2);
  m_disp_vs_plane_scale->m_updated_externally_event = true;

  m_disp_vs_arrow_len = new qbinded_float_spinbox_t;
  m_disp_vs_arrow_len->set_min_max_step(0.01, 20, 0.05);
  m_disp_vs_arrow_len->m_updated_externally_event = true;

  m_disp_vs_arrow_scale = new qbinded_float_spinbox_t;
  m_disp_vs_arrow_scale->set_min_max_step(0.01, 20, 0.05);
  m_disp_vs_arrow_scale->m_updated_externally_event = true;

  m_disp_vs_arrow_cap_len = new qbinded_float_spinbox_t;
  m_disp_vs_arrow_cap_len->set_min_max_step(0.01, 20, 0.05);
  m_disp_vs_arrow_cap_len->m_updated_externally_event = true;

  disp_vs_arrow_cap_scale = new qbinded_float_spinbox_t;
  disp_vs_arrow_cap_scale->set_min_max_step(0.01, 20, 0.05);
  disp_vs_arrow_cap_scale->m_updated_externally_event = true;

  m_disp_vs_plane_alpha = new qbinded_float_spinbox_t;
  m_disp_vs_plane_alpha->set_min_max_step(0.01, 1.0, 0.01);
  m_disp_vs_plane_alpha->m_updated_externally_event = true;

  m_disp_vs_plane_color = new qbinded_color3_input_t;
  m_disp_vs_plane_color->m_updated_externally_event = true;

  m_gb_psg_summary = new qspoiler_widget_t(tr("Symmetry Group Summary"));
  m_gb_psg_summary_lt = new QFormLayout;
  m_gb_psg_summary->add_content_layout(m_gb_psg_summary_lt);
  m_gb_psg_summary_lt->addRow(tr("Sym. gr. name"), m_tg_info_sym_gr);
  m_gb_psg_summary_lt->addRow(tr("Number of"), m_tg_info_numbers);
  m_gb_psg_summary_lt->addRow(tr("Info"), m_tg_info_general);
  init_form_lt(m_gb_psg_summary_lt);

  m_gb_psg_view_settings = new qspoiler_widget_t(tr("View Settings"));
  m_gb_psg_view_settings_lt = new QFormLayout;
  m_gb_psg_view_settings->add_content_layout(m_gb_psg_view_settings_lt);
  m_gb_psg_view_settings_lt->addRow(tr("Show axes"), m_disp_vs_show_axes);
  m_gb_psg_view_settings_lt->addRow(tr("Show planes"), m_disp_vs_show_planes);
  m_gb_psg_view_settings_lt->addRow(tr("Plane transp."), m_disp_vs_plane_alpha_enabled);
  m_gb_psg_view_settings_lt->addRow(tr("Plane scale"), m_disp_vs_plane_scale);
  m_gb_psg_view_settings_lt->addRow(tr("Plane alpha"), m_disp_vs_plane_alpha);
  m_gb_psg_view_settings_lt->addRow(tr("Arrow length"), m_disp_vs_arrow_len);
  m_gb_psg_view_settings_lt->addRow(tr("Arrow scale"), m_disp_vs_arrow_scale);
  m_gb_psg_view_settings_lt->addRow(tr("Cap length"), m_disp_vs_arrow_cap_len);
  m_gb_psg_view_settings_lt->addRow(tr("Cap scale"), disp_vs_arrow_cap_scale);
  m_gb_psg_view_settings_lt->addRow(tr("Plane color"), m_disp_vs_plane_color);
  init_form_lt(m_gb_psg_view_settings_lt);

  //tab colorize orders
  m_tab_display = def_tab("Colorize group elements", "://images/monitor.svg");

  m_gb_colorize = new qspoiler_widget_t(tr("Axis Colors by Order"));
  m_gb_colorize_lt = new QFormLayout;
  m_gb_colorize->add_content_layout(m_gb_colorize_lt);
  for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++) {
      m_gb_colorize_ctrls[i] = new qbinded_color3_input_t;
      m_gb_colorize_ctrls[i]->m_updated_externally_event = true;
      m_gb_colorize_lt->addRow(tr("%1 - order").arg(i+1), m_gb_colorize_ctrls[i]);
    }
  init_form_lt(m_gb_colorize_lt);

  m_tab_general->tab_inner_widget_lt->addWidget(m_gb_psg_summary);
  m_tab_display->tab_inner_widget_lt->addWidget(m_gb_psg_view_settings);
  m_tab_display->tab_inner_widget_lt->addWidget(m_gb_colorize);

  m_tab_general->tab_inner_widget_lt->addStretch();
  m_tab_display->tab_inner_widget_lt->addStretch();

}

void psg_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (_binding_item && _binding_item->get_type() == psg_view_t::get_type_static()) {
      b_pg = _binding_item->cast_as<psg_view_t>();

      m_disp_vs_plane_alpha_enabled->bind_value(&b_pg->m_plane_alpha_enabled, b_pg);
      m_disp_vs_plane_scale->bind_value(&b_pg->m_plane_scale, b_pg);
      m_disp_vs_plane_alpha->bind_value(&b_pg->m_plane_alpha, b_pg);
      m_disp_vs_arrow_len->bind_value(&b_pg->m_arrow_len, b_pg);
      m_disp_vs_arrow_scale->bind_value(&b_pg->m_arrow_scale, b_pg);
      m_disp_vs_arrow_cap_len->bind_value(&b_pg->m_arrow_cap_len, b_pg);
      disp_vs_arrow_cap_scale->bind_value(&b_pg->m_arrow_cap_scale, b_pg);
      m_disp_vs_show_axes->bind_value(&b_pg->m_show_axes);
      m_disp_vs_show_planes->bind_value(&b_pg->m_show_planes);
      m_disp_vs_plane_color->bind_value(&b_pg->m_plane_color, b_pg);

      for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++)
          m_gb_colorize_ctrls[i]->bind_value(&b_pg->m_axes_color_by_order[i], b_pg);

    } else {
      b_pg = nullptr;
    }

  m_tab_display->tab_inner_widget->update();

  update_from_ws_item();

}

void psg_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_pg && b_pg->m_ag) {

      for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++) m_gb_colorize_ctrls[i]->load_value_ex();

      m_tg_info_sym_gr->setText(QString::fromStdString(b_pg->m_ag->m_name));

      m_tg_info_general->setText(tr("Inverted? : %1, Bounded? : %2")
                                .arg(b_pg->m_pg_axes.inversion ? tr("Yes") : tr("No"))
                                .arg(b_pg->is_bounded() ? tr("Yes") : tr("No")));

      m_tg_info_numbers->setText(tr("Ops.: %1, Axes: %2, Planes.: %3")
                                    .arg(b_pg->m_ag->size())
                                    .arg(b_pg->m_pg_axes.axes.size())
                                    .arg(b_pg->m_pg_axes.planes.size()));

    }

}

void psg_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  m_disp_vs_plane_alpha_enabled->unbind_value();
  m_disp_vs_plane_scale->unbind_value();
  m_disp_vs_plane_alpha->unbind_value();

  m_disp_vs_arrow_len->unbind_value();
  m_disp_vs_arrow_scale->unbind_value();
  m_disp_vs_arrow_cap_len->unbind_value();
  disp_vs_arrow_cap_scale->unbind_value();
  m_disp_vs_show_axes->unbind_value();
  m_disp_vs_show_planes->unbind_value();
  m_disp_vs_plane_color->unbind_value();

  for (size_t i = 0; i < AXIS_COLORIZE_SIZE; i++)
      m_gb_colorize_ctrls[i]->unbind_value();

  b_pg = nullptr;

}

