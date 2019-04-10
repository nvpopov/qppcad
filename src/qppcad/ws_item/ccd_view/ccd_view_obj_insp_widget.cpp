#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

void ccd_view_obj_insp_widget_t::bind_to_item(ws_item_t *_binding_item) {

  if (_binding_item && _binding_item->get_type() ==
      ccd_view_t::get_type_static()) {
      ccd_view_t *dp = _binding_item->cast_as<ccd_view_t>();
      if (dp) {
          // std::cout << "accepted ccd_view_t in ::bind_to_item" << std::endl;
          b_ccd = dp;
        }
      else {
          //std::cout << "ALL IS WRONG" << std::endl;
          b_ccd = nullptr;
        }
    }

  ws_item_obj_insp_widget_t::bind_to_item(_binding_item);
}

void ccd_view_obj_insp_widget_t::update_from_ws_item() {

  ws_item_obj_insp_widget_t::update_from_ws_item();

  if (b_ccd) {
      // std::cout << "ALL IS GOOD" << std::endl;
      gen_ccd_info_prog_name->setText(
            QString::fromStdString(
              qpp::ccdprog2str[b_ccd->m_ccd->m_comp_chem_program]
            )
          );
      gen_ccd_info_run_type->setText(
            QString::fromStdString(
              qpp::ccdrt2str[b_ccd->m_ccd->m_run_t]
            )
          );
      switch (b_ccd->m_ccd->m_run_t) {

        case comp_chem_program_run_e::rt_unknown :
          tabBar()->setTabEnabled(1, false);
          tabBar()->setTabEnabled(2, false);
          break;

        case comp_chem_program_run_e::rt_energy :
          tabBar()->setTabEnabled(1, false);
          tabBar()->setTabEnabled(2, false);
          break;

        case comp_chem_program_run_e::rt_geo_opt :
          tabBar()->setTabEnabled(1, true);
          tabBar()->setTabEnabled(2, false);
          update_geo_opt();
          break;

        default :
          break;
        }

      //
    }
}

void ccd_view_obj_insp_widget_t::unbind_item() {
  ws_item_obj_insp_widget_t::unbind_item();
  b_ccd = nullptr;
}

void ccd_view_obj_insp_widget_t::update_geo_opt() {

}

ccd_view_obj_insp_widget_t::ccd_view_obj_insp_widget_t()
  : ws_item_obj_insp_widget_t() {

//  tab_geo_opt = def_tab(tr("Geometry opt."),
//                           "://images/settings.svg",
//                           "://images/settings.svg");
//  tab_vibs = def_tab(tr("Vibrational analysis"),
//                        "://images/settings.svg",
//                        "://images/settings.svg");

  ws_item_is_visible->hide();
  ws_item_is_visible_label->hide();

  gb_gen_ccd_info = new qspoiler_widget_t(tr("CCD info"));
  gb_gen_ccd_info_lt = new QFormLayout;
  gb_gen_ccd_info->add_content_layout(gb_gen_ccd_info_lt);

  gen_ccd_info_prog_name = new QLabel;
  gen_ccd_info_run_type = new QLabel;

  gb_gen_ccd_info_lt->addRow(tr("Program"), gen_ccd_info_prog_name);
  gb_gen_ccd_info_lt->addRow(tr("Run type"), gen_ccd_info_run_type);
  init_form_lt(gb_gen_ccd_info_lt);

  tab_general->tab_inner_widget_lt->addWidget(gb_gen_ccd_info);
  tab_general->tab_inner_widget_lt->addStretch(0);

  //tab_geo_opt->tab_inner_widget_lt->addStretch(0);

  //tg_form_layout->
}
