#include <qppcad/ws_atoms_list/ws_atoms_list_io.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>

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

}
