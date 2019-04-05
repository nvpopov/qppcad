#include <qppcad/ws_item/psg_view/psg_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

psg_view_obj_insp_widget_t::psg_view_obj_insp_widget_t() {

  tg_info_summary_widget = new QGroupBox;
  tg_info_summary_lt = new QFormLayout;

  pre_init_gb(tg_info_summary_widget, tg_info_summary_lt);

  tg_info_summary_lt->setLabelAlignment(Qt::AlignRight);
  tg_info_summary_widget->setLayout(tg_info_summary_lt);

  tg_info_summary_widget->setTitle("Symmetry group summary");
  tab_general->tab_inner_widget_lt->addWidget(tg_info_summary_widget);
  tg_info_sym_gr = new QLabel;
  tg_info_total_sym_op = new QLabel;
  tg_plane_alpha_enabled = new qbinded_checkbox_t;
  cb_show_axes = new qbinded_checkbox_t;
  cb_show_planes = new qbinded_checkbox_t;

  tg_plane_scale = new qbinded_float_spinbox_t;
  tg_plane_scale->set_min_max_step(1, 20, 0.1, 2);
  tg_plane_scale->m_updated_internally_event = true;

  tg_axis_scale = new qbinded_float_spinbox_t;
  tg_axis_scale->set_min_max_step(1, 20, 0.1, 2);
  tg_axis_scale->m_updated_internally_event = true;

  tg_axis_len_mod = new qbinded_float_spinbox_t;
  tg_axis_len_mod->set_min_max_step(0.5, 10, 0.1, 2);
  tg_axis_len_mod->m_updated_internally_event = true;

  tg_info_summary_lt->addRow(tr("Sym. gr. name"), tg_info_sym_gr);
  tg_info_summary_lt->addRow(tr("Num. of op."), tg_info_total_sym_op);
  tg_info_summary_lt->addRow(tr("Show axes"), cb_show_axes);
  tg_info_summary_lt->addRow(tr("Show planes"), cb_show_planes);
  tg_info_summary_lt->addRow(tr("Plane transp."), tg_plane_alpha_enabled);
  tg_info_summary_lt->addRow(tr("Plane scale"), tg_plane_scale);
  tg_info_summary_lt->addRow(tr("Axis scale"), tg_axis_scale);
  tg_info_summary_lt->addRow(tr("Axis length"), tg_axis_len_mod);

  post_init_gb(tg_info_summary_widget, tg_info_summary_lt);

  tab_general->tab_inner_widget_lt->addStretch();

}

void psg_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

  if (_binding_item && _binding_item->get_type() == psg_view_t::get_type_static()) {
      b_pg = _binding_item->cast_as<psg_view_t>();

      if (b_pg && b_pg->m_ag) {
          tg_info_sym_gr->setText(QString::fromStdString(b_pg->m_ag->name));
          tg_info_total_sym_op->setText(tr("%1").arg(b_pg->m_ag->size()));
        }

      tg_plane_alpha_enabled->bind_value(&b_pg->m_plane_alpha_enabled, b_pg);
      tg_plane_scale->bind_value(&b_pg->m_plane_scale, b_pg);
      tg_axis_scale->bind_value(&b_pg->m_axis_scale, b_pg);
      tg_axis_len_mod->bind_value(&b_pg->m_axis_len_mod, b_pg);
      cb_show_axes->bind_value(&b_pg->m_show_axes);
      cb_show_planes->bind_value(&b_pg->m_show_planes);

    } else {
      b_pg = nullptr;
    }

}

void psg_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

}

void psg_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  tg_plane_alpha_enabled->unbind_value();
  tg_plane_scale->unbind_value();
  tg_axis_scale->unbind_value();
  tg_axis_len_mod->unbind_value();
  cb_show_axes->unbind_value();
  cb_show_planes->unbind_value();

  b_pg = nullptr;

}
