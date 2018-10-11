#ifndef QPP_CAD_WS_ATOMS_LIST_MEASURE_S
#define QPP_CAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
#include <qppcad/app.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    template<typename AINT = uint32_t>
    struct measurement_bond_record {
        AINT at1;
        AINT at2;
        index idx1;
        index idx2;

        measurement_bond_record (const AINT _at1, const AINT _a2,
                                 const index _idx1, const index _idx2) :
          at1(_at1), at2(_a2), idx1(_idx1), idx2(_idx2) {}

    };

    template <typename DATA, typename REAL = float, typename AINT  = uint32_t>
    class ws_atoms_list_measurement_subsys_t {

      private:

        DATA* p_owner;
        std::vector<measurement_bond_record<AINT> > m_records;

      public:

        ws_atoms_list_measurement_subsys_t (DATA &_p_owner) {
          p_owner = &_p_owner;
        }

        void add_bond_measurement (const AINT atm1, const AINT atm2,
                                   const index idx1, const index idx2) {
          m_records.emplace_back(atm1, atm2, idx1, idx2);
        }

        void remove_bond_measurement (const size_t measure_idx) {
          if (measure_idx < m_records.size())
            m_records.erase(m_records.begin() + measure_idx);
        }

        std::optional<size_t> is_bond_measurement_exist (const AINT atm1,
                                                          const AINT atm2,
                                                          const index idx1,
                                                          const AINT idx2) const {

          for (auto i = 0; i < m_records.size(); i++)
            if ((m_records[i].at1 == atm1 && m_records[i].at2 == atm2 &&
                m_records[i].idx1 == idx1 && m_records[i].idx2 == idx2) ||
                (m_records[i].at1 == atm2 && m_records[i].at2 == atm1 &&
                 m_records[i].idx1 == idx2 && m_records[i].idx2 == idx1))
              return std::optional<size_t>(i);

          return std::nullopt;

        }

        void render () {
          //render bond measurements
          app_state_t* astate = &(c_app::get_state());
          astate->dp->begin_render_line_styled();

          for (auto &record : m_records) {
              astate->dp->render_line_styled(clr_black, p_owner->m_geom->pos(record.at1, record.idx1),
                                      p_owner->m_geom->pos(record.at2, record.idx2), 6.0f);
            }

          astate->dp->end_render_line_styled();
        }

        void render_ui_context () {

          if (p_owner->m_atom_idx_sel.size() == 2) {
              if (ImGui::BeginMenu("Measurements")) {
                  auto it1 = p_owner->m_atom_idx_sel.begin();
                  auto it2 = ++(p_owner->m_atom_idx_sel.begin());

                  auto cur_sel = is_bond_measurement_exist(it1->m_atm, it2->m_atm,
                                                            it1->m_idx, it2->m_idx);
                  if (cur_sel)
                    if (ImGui::MenuItem("Remove distance measurements"))
                      remove_bond_measurement(*cur_sel);

                  if (!cur_sel)
                    if (ImGui::MenuItem("Add distance measurement"))
                      add_bond_measurement( it1->m_atm, it2->m_atm,
                                            it1->m_idx, it2->m_idx);
                  ImGui::EndMenu();
                }
            }

        } // end render_ui_context
    };

  }

}

#endif
