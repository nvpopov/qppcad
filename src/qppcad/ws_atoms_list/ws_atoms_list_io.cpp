#include <qppcad/ws_atoms_list/ws_atoms_list_io.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void ws_atoms_list_io_cube_t::load_from_stream_ex(std::basic_istream<char, TRAITS> &stream,
                                                  ws_atoms_list_t *_item,
                                                  workspace_t *ws) {

  std::shared_ptr<ws_volume_data_t> vold = std::make_shared<ws_volume_data_t>();
  _item->m_geom->DIM = 0;
  _item->m_geom->cell.DIM = 0;
  vold->load_from_stream(stream, *(_item->m_geom.get()), _item->m_name);
  _item->m_parent_ws->add_item_to_ws(vold);

  if (m_cell_emplace) {
      _item->m_geom->DIM = 3;
      _item->m_geom->cell.DIM = 3;
      _item->m_geom->cell.v[0] = vold->m_volume.m_axis[0] * vold->m_volume.m_steps[0];
      _item->m_geom->cell.v[1] = vold->m_volume.m_axis[1] * vold->m_volume.m_steps[1];
      _item->m_geom->cell.v[2] = vold->m_volume.m_axis[2] * vold->m_volume.m_steps[2];
    }

  // TODO: add camera fix

}
