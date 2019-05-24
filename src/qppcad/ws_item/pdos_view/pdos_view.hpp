#ifndef QPP_CAD_PDOS_VIEW
#define QPP_CAD_PDOS_VIEW
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <io/pdos.hpp>
#include <io/ccd_programs.hpp>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#pragma pop_macro("slots")

QT_CHARTS_USE_NAMESPACE

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class pdos_view_t : public ws_item_t {

        QPP_OBJECT(pdos_view_t, ws_item_t)

      public:

        std::vector<pdos_data_t<float> > m_pdos_recs;
        float m_pdos_ewindow_low{0};
        float m_pdos_ewindow_high{1};

        QChart *m_pdos_gen_chart;
        QChartView *m_pdos_chart_view;

        pdos_view_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_externally(uint32_t update_reason) override;
        uint32_t get_num_cnt_selected() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data, repair_connection_info_t &rep_info) override;

        void add_data_from_file(const std::string &file_name, comp_chem_program_e ccd_prog);
        void rebuild_plots();

        size_t get_num_species();
        size_t get_num_channels();
        bool is_spin_polarized();

        float py_get_efermi_for_channel(size_t channel_idx);
        void scale_channel(size_t channel_idx, float magn);
        void py_load_from_list(py::list _pdos_files, comp_chem_program_e _ccd_prog);
        void py_load_from_dir(std::string _dir,
                              std::string _pattern,
                              comp_chem_program_e _ccd_prog);

    };

  } // namespace cad

} // namespace qpp

#endif
