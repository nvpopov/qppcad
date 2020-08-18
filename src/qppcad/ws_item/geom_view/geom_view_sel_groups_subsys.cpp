#include <qppcad/ws_item/geom_view/geom_view_sel_groups_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

geom_view_sel_groups_subsys_t::geom_view_sel_groups_subsys_t(geom_view_t &_p_owner) {

  p_owner = &_p_owner;
  m_auto_apply.set_value(false); add_hs_child(&m_auto_apply);

}

void geom_view_sel_groups_subsys_t::make_from_selected() {

  if (!p_owner || p_owner->m_geom->no_aselected()) return;

  geom_view_sel_group_t gvs_tmp(fmt::format("from_sel{}", m_sel_grps.size()));

  for (auto i = 0; i < p_owner->m_geom->num_selected(); i++) {
    auto rec = p_owner->m_geom->nth_aselected(i);
    if (rec) gvs_tmp.append((*rec));
  }

  m_sel_grps.push_back(std::move(gvs_tmp));

}

void geom_view_sel_group_t::append(atom_index_set_key new_rec) {

  m_data.push_back(new_rec);

}

void geom_view_sel_group_t::remove(size_t idx) {

  if (idx < m_data.size()) m_data.erase(m_data.begin() + idx);

}
