#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <io/cp2k_pdos.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

pdos_view_t::pdos_view_t() {

  set_default_flags(ws_item_flags_default);
  m_pdos_gen_chart = new QChart();

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

void pdos_view_t::add_data_from_file(const std::string &file_name, comp_chem_program_e ccd_prog) {

  bool succes{false};
  auto _file_name = file_name;

  pdos_data_t<float> pdos_rec;
  std::ifstream pdos_file(_file_name);

  switch (ccd_prog) {
    case comp_chem_program_e::pr_cp2k: {
        read_cp2k_pdos(_file_name, pdos_file, pdos_rec);
        succes = true;
        break;
      }
    default:
      break;
    }

  if (succes) m_pdos_recs.push_back(std::move(pdos_rec));

}

void pdos_view_t::rebuild_plots() {

  app_state_t *astate = app_state_t::get_inst();

  m_pdos_gen_chart->removeAllSeries();

  for (auto &rec : m_pdos_recs) {

      QLineSeries* series = new QLineSeries();

      auto last_e = rec.m_data[0](0,0);
      const auto last_e_th = 0.01;

      for (auto &inner_rec : rec.m_data) {

          float cur_e = inner_rec(0,0);
          int zr_steps = 5;
          float d_e = cur_e - last_e;
          float dd_e = d_e / zr_steps;

          if (d_e > last_e_th) {
              for (size_t i = 0; i > 5; i++)
                series->append(cur_e + dd_e * i, 0);
            }

          // sum over occupancy
          float occ = 0;
          for (size_t i = 2; i < inner_rec.size(); i++) occ += inner_rec(0,i);
          astate->tlog("@PLOT_BUILD {} {}", cur_e, rec.m_is_alpha ? occ : -occ);
          series->append(cur_e,rec.m_spin_polarized ?(rec.m_is_alpha ? occ : -occ) : occ);
          last_e = cur_e;

        }

      series->setName(QString("%1 %2")
                      .arg(QString::fromStdString(rec.m_specie_name))
                      .arg(rec.m_spin_polarized ?
                             rec.m_is_alpha ? "(↑)" : "(↓)" :
                                              "(↑↓)")
                      );
      m_pdos_gen_chart->addSeries(series);

    }

  m_pdos_gen_chart->createDefaultAxes();

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

void pdos_view_t::py_load_from_list(py::list _pdos_files, comp_chem_program_e _ccd_progs) {

  bool need_to_rebuild_plots{false};

  for (size_t i = 0; i < _pdos_files.size(); i++)
    if (py::isinstance<py::str>(_pdos_files[i])) {
        need_to_rebuild_plots = true;
        std::string file_name = py::cast<std::string>(_pdos_files[i]);
        add_data_from_file(file_name, _ccd_progs);
      }

  if (need_to_rebuild_plots) rebuild_plots();

}

void pdos_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);
  rebuild_plots();

}

