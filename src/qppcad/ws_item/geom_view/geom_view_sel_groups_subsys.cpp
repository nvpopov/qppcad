#include <qppcad/ws_item/geom_view/geom_view_sel_groups_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

geom_view_sel_groups_subsys_t::geom_view_sel_groups_subsys_t(geom_view_t &_p_owner) {

  p_owner = &_p_owner;

}

void geom_view_sel_groups_subsys_t::make_from_selected() {

  if (!p_owner || p_owner->m_atom_idx_sel.empty()) return;

  geom_view_sel_group_t _tmp(fmt::format("from_sel{}", m_sel_grps.size()));
  for (auto &rec : p_owner->m_atom_idx_sel) _tmp.append(rec);

  m_sel_grps.push_back(std::move(_tmp));

}

void geom_view_sel_group_t::append(atom_index_set_key new_rec) {

  m_data.push_back(new_rec);

}

void geom_view_sel_group_t::remove(size_t idx) {

  if (idx < m_data.size()) m_data.erase(m_data.begin() + idx);

}
