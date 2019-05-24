#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <io/cp2k_pdos.hpp>
#include <qppcad/app_state.hpp>
#include <QDir>
#include <QFileInfo>

using namespace qpp;
using namespace qpp::cad;

pdos_view_t::pdos_view_t() {

  set_default_flags(ws_item_flags_default);
  m_pdos_gen_chart = new QChart();
  m_pdos_chart_view = new QChartView();
  m_pdos_chart_view->setRubberBand(QChartView::HorizontalRubberBand);
  m_pdos_chart_view->setChart(m_pdos_gen_chart);
  m_pdos_chart_view->setRenderHint(QPainter::Antialiasing);

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

  float pdos_sigma = 0.05f;

  for (auto &rec : m_pdos_recs) {

      bool _rec_is_spin_polarized = rec.m_is_alpha != rec.m_is_beta;

      // with gaussian smearing
      float e_min = rec.m_data.front()(0,0);
      float e_max = rec.m_data.back()(0,0);

      int steps = 5000;
      float e_step = (e_max - e_min) / float(steps);
      int e_cnt = rec.m_data.size();

//      astate->tlog("@PDOS_SMEAR: e_min={}, e_max={}, e_step={}, e_cnt={}",
//                   e_min, e_max, e_step, e_cnt);

      using pda_t = Eigen::Array<float, 1, Eigen::Dynamic>;

      pda_t _per_data;
      _per_data.resize(Eigen::NoChange, steps);
      _per_data = 0;

      std::vector<pda_t> _per_e;
      _per_e.resize(e_cnt);

      // generate arrays per eigenstate
      // threaded version
      for (size_t i = 0; i < e_cnt; i++) {

          _per_e[i].resize(Eigen::NoChange, steps);
          _per_e[i] = 0;
          float e_0 = rec.m_data[i](0,0);

          //compute amplitude - i.e. m_l occupancy
          float occ{0};
          for (size_t occ_i = 2; occ_i < rec.m_data[i].cols(); occ_i++)
            occ += rec.m_data[i](0, occ_i);

//          astate->tlog("@PDOS_SMEAR: i={}, occ={}, e_0={}", i, occ, e_0);

          //generate gaussian
          for (size_t q = 0; q < steps; q++) {
              float e_q = e_min + e_step * q;
              _per_e[i](0, q) =
                  (1 / (pdos_sigma * (std::sqrt(2*qpp::pi)))) * occ * std::exp(
                    -0.5f * std::pow((e_q - e_0)/pdos_sigma, 2)
                    );
            }

        }

      // compose generated arrays
      float compose_coeff = 1.0f;
      if (_rec_is_spin_polarized && rec.m_is_beta) compose_coeff = -1.0f;
      for (size_t i = 0; i < e_cnt; i++) _per_data += compose_coeff*_per_e[i];

      // compose QLineSeries from "composed" array
      QLineSeries* series = new QLineSeries();
      for (size_t i = 1; i < steps - 1; i++) series->append(e_min + e_step * i, _per_data(0, i));

      series->setName(QString("%1 %2")
                      .arg(QString::fromStdString(rec.m_specie_name))
                      .arg(_rec_is_spin_polarized ?
                             (rec.m_is_alpha ? "(↑)" : "(↓)"):
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
    if (rec.m_is_alpha != rec.m_is_beta) return true;

  return false;

}

void pdos_view_t::scale_channel(size_t channel_idx, float magn) {

  if (channel_idx >= m_pdos_recs.size()) return;

  auto &pd_rec = m_pdos_recs[channel_idx];

  for (size_t eigv_i = 0; eigv_i < pd_rec.m_data.size(); eigv_i++)
    for (size_t occ_i = 2; occ_i < pd_rec.m_data[eigv_i].cols(); occ_i++)
      pd_rec.m_data[eigv_i](0, occ_i) *= magn;

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

void pdos_view_t::py_load_from_dir(std::string _dir,
                                   std::string _pattern,
                                   comp_chem_program_e _ccd_prog) {

  app_state_t *astate = app_state_t::get_inst();

  auto promted_dir = QDir(QString::fromStdString(_dir));
  auto files_info = promted_dir.entryInfoList();

  QString qpattern = QString::fromStdString(_pattern);

  for (auto &file_entry : files_info) {
      astate->tlog("@PDOS_VIEW_T::PY_LOAD_DIR file_name = {}",
                   file_entry.absoluteFilePath().toStdString());
      if (file_entry.fileName().contains(qpattern))
        add_data_from_file(file_entry.absoluteFilePath().toStdString(), _ccd_prog);
    }

  rebuild_plots();

}

void pdos_view_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);
  rebuild_plots();

}

