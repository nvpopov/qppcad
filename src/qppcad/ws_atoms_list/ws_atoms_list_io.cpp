#include <qppcad/ws_atoms_list/ws_atoms_list_io.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <io/molcas.hpp>
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


  //TODO: Fix cell emplace
  //  if (m_cell_emplace) {
  //      _item->m_geom->DIM = 3;
  //      _item->m_geom->cell.DIM = 3;
  //      _item->m_geom->cell.v[0] = vold->m_volume.m_axis[0] * vold->m_volume.m_steps[0];
  //      _item->m_geom->cell.v[1] = vold->m_volume.m_axis[1] * vold->m_volume.m_steps[1];
  //      _item->m_geom->cell.v[2] = vold->m_volume.m_axis[2] * vold->m_volume.m_steps[2];
  //    }

  // TODO: add camera fix

}

void ws_atoms_list_molcas_grid_t::load_from_stream_ex(std::basic_istream<char, TRAITS> &stream,
                                                      ws_atoms_list_t *_item,
                                                      workspace_t *ws) {
  std::shared_ptr<ws_volume_data_t> vold = std::make_shared<ws_volume_data_t>();
  _item->m_geom->DIM = 0;
  _item->m_geom->cell.DIM = 0;

  std::vector<scalar_volume_t<float>> tmp_volumes;
  load_grid_ascii(stream, *(_item->m_geom.get()), tmp_volumes);
  _item->m_parent_ws->add_item_to_ws(vold);
  vold->m_name = fmt::format("{}.volume", _item->m_name);

  //process volumes
  vold->m_volumes.resize(tmp_volumes.size());
  for (size_t i = 0; i < tmp_volumes.size(); i++) {
      vold->m_volumes[i].m_volume = std::move(tmp_volumes[i]);
      vold->m_volumes[i].m_ready_to_render = false;
      vold->m_volumes[i].m_need_to_regenerate = true;
      if (vold->m_volumes[i].m_volume.m_has_negative_values) {
          vold->m_volumes[i].m_volume_type = ws_volume_t::volume_mo;
          vold->m_volumes[i].m_isolevel = qpp::def_isovalue_mo;
        } else {
          vold->m_volumes[i].m_volume_type = ws_volume_t::volume_density;
          vold->m_volumes[i].m_isolevel = qpp::def_isovalue_dens;
        }
    }

  for (size_t i = 0; i < vold->m_volumes.size(); i++)
    fmt::print(std::cout,
               "VOLUME {} NUM POINTS {}\n", i, vold->m_volumes[i].m_volume.m_field.size());
}
