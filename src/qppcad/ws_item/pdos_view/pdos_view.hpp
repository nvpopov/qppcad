#ifndef QPP_CAD_PDOS_VIEW
#define QPP_CAD_PDOS_VIEW
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <io/pdos.hpp>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace qpp {

  namespace cad {

    class pdos_view_t : public ws_item_t {

        QPP_OBJECT(pdos_view_t, ws_item_t)

      public:

        std::vector<pdos_data_t<float> > m_pdos_recs;
        float m_pdos_ewindow_low{0};
        float m_pdos_ewindow_high{1};

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

        void add_data_from_file(const std::string &file_name);

        size_t get_num_species();
        size_t get_num_channels();
        bool is_spin_polarized();

    };

  } // namespace cad

} // namespace qpp

#endif
