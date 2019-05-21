#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

pdos_view_t::pdos_view_t() {

  set_default_flags(ws_item_flags_default);
  pdos_gen_chart = new QChart();

}

void pdos_view_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void pdos_view_t::render() {
 //do nothing
}

bool pdos_view_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string pdos_view_t::compose_type_descr() {
  return "pdos_view";
}

void pdos_view_t::update(float delta_time) {
  //do nothing
}

float pdos_view_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t pdos_view_t::get_num_cnt_selected() {
  return 0;
}

size_t pdos_view_t::get_content_count() {
  return 0;
}

void pdos_view_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void pdos_view_t::load_from_json(json &data, repair_connection_info_t &rep_info) {
  ws_item_t::load_from_json(data, rep_info);
}

void pdos_view_t::add_data_from_file(const std::string &file_name) {

  bool succes{false};

  pdos_data_t<float> pdos_rec;
  std::ifstream pdos_file(file_name);

//  if (res == m_data_target_cp2k) {
//      read_cp2k_pdos(_file_name, pdos_file, pdos_rec);
//      succes = true;
//    } else if (res == m_data_target_vasp) {

//    }

//  if (succes && m_pdv) m_pdv->m_pdos_recs.push_back(std::move(pdos_rec));

}

void pdos_view_t::rebuild_plots() {

  pdos_gen_chart->removeAllSeries();

  for (auto &rec : m_pdos_recs) {
       QLineSeries* series = new QLineSeries();
       //series->setUseOpenGL(true);
       for (auto &inner_rec : rec.m_data)
         if (inner_rec(0,0) > m_pdos_ewindow_low && inner_rec(0,0) < m_pdos_ewindow_high) {
           // sum over occupancy
           float occ = 0;
           for (size_t i = 2; i < inner_rec.size(); i++)
             occ += inner_rec(0,i);
           series->append(inner_rec(0,0), rec.m_is_alpha ? occ : -occ);
         }
       series->setName(QString("%1 %2")
                       .arg(QString::fromStdString(rec.m_specie_name))
                       .arg(rec.m_is_alpha ? "↑" : "↓")
                       );
       pdos_gen_chart->addSeries(series);
    }

  pdos_gen_chart->createDefaultAxes();

}

size_t pdos_view_t::get_num_species() {

  std::set<size_t> species;
  for (auto &rec : m_pdos_recs) species.insert(rec.m_atom_kind);

  return species.size();

}

size_t pdos_view_t::get_num_channels() {

  return m_pdos_recs.size();

}

bool pdos_view_t::is_spin_polarized() {

  for (auto &rec : m_pdos_recs)
    if (rec.m_spin_polarized) return true;

  return false;

}

void pdos_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

