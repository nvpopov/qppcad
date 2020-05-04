#ifndef QPPCAD_GEOM_VIEW_SEL_GROUPS_SUBSYS
#define QPPCAD_GEOM_VIEW_SEL_GROUPS_SUBSYS

#include <qppcad/core/qppcad.hpp>
#include <symm/index_set.hpp>
#include <qppcad/ws_item/ws_item_subsystem.hpp>

namespace qpp {

  namespace cad {

    class geom_view_t;

    struct geom_view_sel_group_t {

      size_t m_nested_dim{0};
      bool m_failure_on_wrong_dim{false};

      std::vector<atom_index_set_key> m_data;
      std::string m_name;

      geom_view_sel_group_t(const std::string &_name) : m_name(_name){}
      void append(atom_index_set_key new_rec);
      void remove(size_t idx);

    };

    class geom_view_sel_groups_subsys_t : public ws_item_subsystem_t{

      QPP_OBJECT(geom_view_sel_groups_subsys_t, ws_item_subsystem_t)

      private:

        geom_view_t* p_owner;

      public:

        std::vector<geom_view_sel_group_t> m_sel_grps;
        int m_cur_sel_grp_ui{0}; // 0 - none, real = +1
        hs_prop_bool_t m_auto_apply;

        geom_view_sel_groups_subsys_t (geom_view_t &_p_owner);
        void make_from_selected();

    };

  } // namespace cad

} // namespace qpp

#endif
