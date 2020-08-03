#include <qppcad/ws_item/geom_view/geom_view_io.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <io/molcas.hpp>
#include <io/vasp_chgcar.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void geom_view_io_cube_t::load_from_stream_ex(std::basic_istream<char, TRAITS> &stream,
                                              geom_view_t *item,
                                              workspace_t *ws) {

  std::shared_ptr<volume_view_t> vold = std::make_shared<volume_view_t>();

  item->m_geom->set_DIM(0);
  //item->m_geom->cell.DIM = 0;

  ws_volume_record_t new_vol_rec;

  read_cube(stream, *item->m_geom, new_vol_rec.m_volume);
  new_vol_rec.m_owner = vold.get();
  new_vol_rec.m_need_to_regenerate = true;
  new_vol_rec.m_ready_to_render = false;

  vold->m_name.set_value(fmt::format("v_{}", item->m_name.get_value()));
  vold->m_genesis_file_name = item->m_genesis_file_name;

  if (new_vol_rec.m_volume.m_has_negative_values) {
    new_vol_rec.m_volume_type = ws_volume_t::volume_mo;
    new_vol_rec.m_isolevel.set_value(qpp::def_isovalue_mo);
  } else {
    new_vol_rec.m_volume_type = ws_volume_t::volume_density;
    new_vol_rec.m_isolevel.set_value(qpp::def_isovalue_dens);
  }

  auto state_spin_ssp = geom_view_io_helpers_t::extract_state_spin_subspace_from_name(
      vold->m_genesis_file_name.get_value());

  if (state_spin_ssp) {
    new_vol_rec.m_state_id = std::get<0>(*state_spin_ssp);
    new_vol_rec.m_spin_subspace = std::get<1>(*state_spin_ssp);
  }

  new_vol_rec.m_volume.m_name = "From CUBE";

  auto new_vol_rec_sp = std::make_shared<ws_volume_record_t>(std::move(new_vol_rec));
  vold->m_volumes.push_back(new_vol_rec_sp);

  item->m_parent_ws->add_item_to_ws(vold);

}

void geom_view_molcas_grid_t::load_from_stream_ex(std::basic_istream<char, TRAITS> &stream,
                                                  geom_view_t *item,
                                                  workspace_t *ws) {

  std::shared_ptr<volume_view_t> vold = std::make_shared<volume_view_t>();
  item->m_geom->set_DIM(0);
  //item->m_geom->cell.DIM = 0;

  std::vector<scalar_volume_t<float> > tmp_volumes;
  load_grid_ascii(stream, *(item->m_geom.get()), tmp_volumes);
  item->m_parent_ws->add_item_to_ws(vold);
  vold->m_name.set_value(fmt::format("{}.volume", item->m_name.get_value()));
  vold->m_genesis_file_name = item->m_genesis_file_name;

  //process volumes
  vold->m_volumes.reserve(tmp_volumes.size());

  for (size_t i = 0; i < tmp_volumes.size(); i++) {

    ws_volume_record_t new_vol_rec;

    new_vol_rec.m_owner = vold.get();

    new_vol_rec.m_volume = std::move(tmp_volumes[i]);
    new_vol_rec.m_ready_to_render = false;
    new_vol_rec.m_need_to_regenerate = true;

    if (new_vol_rec.m_volume.m_has_negative_values) {
      new_vol_rec.m_volume_type = ws_volume_t::volume_mo;
      new_vol_rec.m_isolevel.set_value(qpp::def_isovalue_mo);
    } else {
      new_vol_rec.m_volume_type = ws_volume_t::volume_density;
      new_vol_rec.m_isolevel.set_value(qpp::def_isovalue_dens);
    }

    auto new_vol_rec_ptr = std::make_shared<ws_volume_record_t>(std::move(new_vol_rec));

    vold->m_volumes.push_back(new_vol_rec_ptr);

  } // end for

}

void geom_view_vasp_chgcar_t::load_from_stream_ex(std::basic_istream<char, TRAITS> &stream,
                                                  geom_view_t *item,
                                                  workspace_t *ws) {

  std::shared_ptr<volume_view_t> vold = std::make_shared<volume_view_t>();
  item->m_geom->set_DIM(3);
  //item->m_geom->cell.DIM = 3;

  std::vector<scalar_volume_t<float> > tmp_volumes;
  read_vasp_chgcar(stream, *(item->m_geom.get()), tmp_volumes);
  item->m_parent_ws->add_item_to_ws(vold);
  vold->m_name.set_value(fmt::format("{}.volume", item->m_name.get_value()));
  vold->m_genesis_file_name = item->m_genesis_file_name;

  vold->m_volumes.reserve(tmp_volumes.size());

  for (size_t i = 0; i < tmp_volumes.size(); i++) {

    ws_volume_record_t new_vol_rec;

    new_vol_rec.m_owner = vold.get();

    new_vol_rec.m_volume = std::move(tmp_volumes[i]);
    new_vol_rec.m_ready_to_render = false;
    new_vol_rec.m_need_to_regenerate = true;

    if (new_vol_rec.m_volume.m_has_negative_values) {
      new_vol_rec.m_volume_type = ws_volume_t::volume_mo;
      new_vol_rec.m_isolevel.set_value(qpp::def_isovalue_mo);
    } else {
      new_vol_rec.m_volume_type = ws_volume_t::volume_density;
      new_vol_rec.m_isolevel.set_value(qpp::def_isovalue_dens);
    }

    auto new_vol_rec_ptr = std::make_shared<ws_volume_record_t>(std::move(new_vol_rec));

    vold->m_volumes.push_back(new_vol_rec_ptr);

  } // end for

}

std::optional<std::tuple<size_t, size_t>>
geom_view_io_helpers_t::extract_state_spin_subspace_from_name(const std::string &file_name) {

  /*
    cp2k style name
    "bbi-WFN_00582_2-1_0.cube"
  */
  if (auto wfn_pos = file_name.find("-WFN_"); wfn_pos != std::string::npos) {

    auto underc_pos = file_name.find("_", wfn_pos);
    auto mpos = file_name.find("_", underc_pos + 5);
    auto substr_state = file_name.substr(wfn_pos + 5, 5);
    auto spin_subsp = file_name.substr(mpos + 1, 1);

    //app_state_t::get_inst()->tlog("@@@ EXTRACT {} {}", substr_state, spin_subsp);

    return std::optional<std::tuple<size_t, size_t> >({
        std::stoul(substr_state),
        std::stoul(spin_subsp)
    });

  }

  /*
   * orca style name
   * orca.mo394a.cube
  */
  if (auto dot_mo_pos = file_name.find(".mo"); dot_mo_pos != std::string::npos) {

    auto dot_cube_pos = file_name.find(".cube");
    auto substr_state = file_name.substr(dot_mo_pos + 3, dot_cube_pos - dot_mo_pos - 3);
    auto has_a_dot_cube = file_name.find("a.cube") != std::string::npos;
    //auto has_b_dot_cube = file_name.find("b.cube") != std::string::npos;

    return std::optional<std::tuple<size_t, size_t> >({
        std::stoul(substr_state),
        has_a_dot_cube ? 1 : 2
    });

  }

  return std::nullopt;

}
