#include <qppcad/ws_item/psg_view/psg_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

psg_view_obj_insp_widget_t::psg_view_obj_insp_widget_t() {

  gb_psg_summary = new qspoiler_widget_t(tr("Symmetry group summary"));
  gb_psg_summary_lt = new QFormLayout;
  gb_psg_summary->add_content_layout(gb_psg_summary_lt);

  gb_psg_summary_lt->setLabelAlignment(Qt::AlignRight);
  gb_psg_summary->setLayout(gb_psg_summary_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_psg_summary);

  tg_info_sym_gr = new QLabel;
  tg_info_total_sym_op = new QLabel;
  tg_bounded_info = new QLabel;
  tg_plane_alpha_enabled = new qbinded_checkbox_t;
  cb_show_axes = new qbinded_checkbox_t;
  cb_show_planes = new qbinded_checkbox_t;

  tg_plane_scale = new qbinded_float_spinbox_t;
  tg_plane_scale->set_min_max_step(1, 20, 0.1, 2);
  tg_plane_scale->m_updated_externally_event = true;

  tg_arrow_len = new qbinded_float_spinbox_t;
  tg_arrow_len->set_min_max_step(0.01, 20, 0.05);
  tg_arrow_len->m_updated_externally_event = true;

  tg_arrow_scale = new qbinded_float_spinbox_t;
  tg_arrow_scale->set_min_max_step(0.01, 20, 0.05);
  tg_arrow_scale->m_updated_externally_event = true;

  tg_arrow_cap_len = new qbinded_float_spinbox_t;
  tg_arrow_cap_len->set_min_max_step(0.01, 20, 0.05);
  tg_arrow_cap_len->m_updated_externally_event = true;

  tg_arrow_cap_scale = new qbinded_float_spinbox_t;
  tg_arrow_cap_scale->set_min_max_step(0.01, 20, 0.05);
  tg_arrow_cap_scale->m_updated_externally_event = true;

  gb_psg_summary_lt->addRow(tr("Bounded ?"), tg_bounded_info);
  gb_psg_summary_lt->addRow(tr("Sym. gr. name"), tg_info_sym_gr);
  gb_psg_summary_lt->addRow(tr("Num. of op."), tg_info_total_sym_op);
  gb_psg_summary_lt->addRow(tr("Show axes"), cb_show_axes);
  gb_psg_summary_lt->addRow(tr("Show planes"), cb_show_planes);
  gb_psg_summary_lt->addRow(tr("Plane transp."), tg_plane_alpha_enabled);
  gb_psg_summary_lt->addRow(tr("Plane scale"), tg_plane_scale);
  gb_psg_summary_lt->addRow(tr("Arrow length"), tg_arrow_len);
  gb_psg_summary_lt->addRow(tr("Arrow scale"), tg_arrow_scale);
  gb_psg_summary_lt->addRow(tr("Cap length"), tg_arrow_cap_len);
  gb_psg_summary_lt->addRow(tr("Cap scale"), tg_arrow_cap_scale);

  init_form_lt(gb_psg_summary_lt);

  tab_general->tab_inner_widget_lt->addStretch();

}

void psg_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item && _binding_item->get_type() == psg_view_t::get_type_static()) {
      b_pg = _binding_item->cast_as<psg_view_t>();

      if (b_pg && b_pg->m_ag) {
          tg_info_sym_gr->setText(QString::fromStdString(b_pg->m_ag->name));
          tg_info_total_sym_op->setText(tr("%1").arg(b_pg->m_ag->size()));
        }

      tg_plane_alpha_enabled->bind_value(&b_pg->m_plane_alpha_enabled, b_pg);
      tg_plane_scale->bind_value(&b_pg->m_plane_scale, b_pg);
      tg_arrow_len->bind_value(&b_pg->m_arrow_len, b_pg);
      tg_arrow_scale->bind_value(&b_pg->m_arrow_scale, b_pg);
      tg_arrow_cap_len->bind_value(&b_pg->m_arrow_cap_len, b_pg);
      tg_arrow_cap_scale->bind_value(&b_pg->m_arrow_cap_scale, b_pg);
      cb_show_axes->bind_value(&b_pg->m_show_axes);
      cb_show_planes->bind_value(&b_pg->m_show_planes);

    } else {
      b_pg = nullptr;
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);

}

void psg_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_pg) {

      tg_bounded_info->setText(b_pg->is_bounded() ? tr("Yes") : tr("No"));

    }

}

void psg_view_obj_insp_widget_t::unbind_item() {

  ws_item_obj_insp_widget_t::unbind_item();

  tg_plane_alpha_enabled->unbind_value();
  tg_plane_scale->unbind_value();
  tg_arrow_len->unbind_value();
  tg_arrow_scale->unbind_value();
  tg_arrow_cap_len->unbind_value();
  tg_arrow_cap_scale->unbind_value();
  cb_show_axes->unbind_value();
  cb_show_planes->unbind_value();

  b_pg = nullptr;

}

