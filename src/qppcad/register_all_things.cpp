#include <qppcad/register_all_things.hpp>

#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view_io.hpp>

#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>

#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view_obj_insp_widget.hpp>

#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view_obj_insp_widget.hpp>

#include <qppcad/ws_item/traj_hl/traj_hl.hpp>
#include <qppcad/ws_item/traj_hl/traj_hl_obj_insp_widget.hpp>

#include <qppcad/ws_item/cube_primitive/cube_primitive.hpp>
#include <qppcad/ws_item/cube_primitive/cube_primitive_obj_insp_widget.hpp>

#include <qppcad/ws_item/arrow_primitive/arrow_primitive.hpp>
#include <qppcad/ws_item/arrow_primitive/arrow_primitive_obj_insp_widget.hpp>

#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/tools/axial_scale/axial_scale.hpp>
#include <qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.hpp>
#include <qppcad/tools/colorize_by_xfield/colorize_by_xfield.hpp>
#include <qppcad/tools/structure_similarity/structure_similarity.hpp>

#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.hpp>

#include <io/write_coord.hpp>

using namespace qpp;
using namespace qpp::cad;

void registration_helper_t::reg_ws_item_fbr(ws_item_behaviour_manager_t *bhv_mgr) {

  reg_ws_item_fbr<geom_view_t>(bhv_mgr);
  reg_ws_item_fbr<ccd_view_t>(bhv_mgr);
  reg_ws_item_fbr<volume_view_t>(bhv_mgr);
  reg_ws_item_fbr<psg_view_t>(bhv_mgr);
  reg_ws_item_fbr<traj_hl_t>(bhv_mgr);
  reg_ws_item_fbr<cube_primitive_t>(bhv_mgr);
  reg_ws_item_fbr<arrow_primitive_t>(bhv_mgr);
  reg_ws_item_fbr<pgf_producer_t>(bhv_mgr);

}

void registration_helper_t::reg_ws_item_obj_insp(ws_item_behaviour_manager_t *bhv_mgr) {

  reg_ws_item_obj_insp_fbr<geom_view_t, geom_view_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<volume_view_t, volume_view_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<ccd_view_t, ccd_view_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<psg_view_t, psg_view_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<traj_hl_t, traj_hl_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<cube_primitive_t, cube_primitive_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<arrow_primitive_t, arrow_primitive_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<pgf_producer_t, pgf_producer_obj_insp_widget_t>(bhv_mgr);

}

void registration_helper_t::reg_ws_item_io_bhv(ws_item_behaviour_manager_t *bhv_mgr) {

  size_t xyz_ff_g_hash = bhv_mgr->reg_ffg("XYZ", "xyz");
  size_t vasp_ff_g_hash = bhv_mgr->reg_ffg("VASP", "vasp");
  size_t firefly_ff_g_hash = bhv_mgr->reg_ffg("Firefly", "ff");
  size_t cp2k_ff_g_hash = bhv_mgr->reg_ffg("CP2K", "cp2k");
  size_t generic_ff_g_hash = bhv_mgr->reg_ffg("Generic formats", "generic");

  size_t xyz_ff_hash =
      bhv_mgr->reg_ff("Standart XYZ", "xyz", xyz_ff_g_hash, {".xyz"});

  size_t xyzq_ff_hash =
      bhv_mgr->reg_ff("UC(XYZQ)", "uc", xyz_ff_g_hash, {".uc"});

  size_t poscar_ff_hash =
      bhv_mgr->reg_ff("VASP POSCAR", "poscar", vasp_ff_g_hash, {"POSCAR", ".vasp", ".VASP"} );

  size_t outcar_ff_hash =
      bhv_mgr->reg_ff("VASP OUTCAR", "outcar", vasp_ff_g_hash, {"OUTCAR"} );

  size_t firefly_out_ff_hash =
      bhv_mgr->reg_ff("FF OUTPUT", "ffout", firefly_ff_g_hash, {".out", ".ff"} );

  size_t cp2k_out_ff_hash =
      bhv_mgr->reg_ff("CP2K OUTPUT", "cp2k", cp2k_ff_g_hash, {"cp2k", ".cout"} );

  size_t cp2k_cs_ff_hash =
      bhv_mgr->reg_ff("CP2K Coord.", "cp2kcs", cp2k_ff_g_hash, {".coord", ".cp2k_crd"} );

  size_t generic_cube_ff_hash =
      bhv_mgr->reg_ff("CUBE file", "cube", generic_ff_g_hash, {".cube", ".CUBE"} );

  size_t generic_cube3d_ff_hash =
      bhv_mgr->reg_ff("CUBE file(3d)", "cube3d", generic_ff_g_hash, {".cube", ".CUBE"} );

  size_t generic_molcas_grid_ff_hash =
      bhv_mgr->reg_ff("Molcas ASCII Grid", "gv", generic_ff_g_hash, {".grid", ".GRID"} );

  size_t generic_raw_coord_ff_hash =
      bhv_mgr->reg_ff("Simple coord.", "coord", generic_ff_g_hash, {"coord", "coord"} );

  auto xyz_ff_mgr =
      std::make_shared<
      geom_view_io_ccd_t<
      read_ccd_from_xyz_file<float>, true, false, true, false, false >
      >();

  auto xyz_s_ff_mgr =
      std::make_shared<
      geom_view_io_saver_t<
      write_xyz<float, periodic_cell<float> > >
      >();

  auto xyzq_mgf =
      std::make_shared<
      geom_view_io_loader_t<
      read_xyzq_wrp_def<float, periodic_cell<float> >, geom_view_role_e::r_uc >
      >();

  auto xyzq_s_mgf =
      std::make_shared<
      geom_view_io_saver_t<
      write_xyzq<float, periodic_cell<float> >, true, 3 >
      >();

  auto ff_output_mgf =
      std::make_shared<
      geom_view_io_ccd_t<
      read_ccd_from_firefly_output<float>, true, true, true, true, false, 0 >
      >();

  auto cp2k_output_mgf =
      std::make_shared<
      geom_view_io_ccd_t<
      read_ccd_from_cp2k_output<float>, true, true, true, false, true >
      >();

  auto cp2k_cs_mgf =
      std::make_shared<
      geom_view_io_saver_t<
      write_cp2k_coord_section<float, periodic_cell<float> >, true, 3 >
      >();

  auto vasp_poscar_mgf =
      std::make_shared<
      geom_view_io_loader_t<
      read_vasp_poscar<float, periodic_cell<float> >, geom_view_role_e::r_generic, 3 >
      >();

  auto vasp_poscar_s_mgf =
      std::make_shared<
      geom_view_io_saver_t<
      write_vasp_poscar<
      float, periodic_cell<float> >, true, 3 >
      >();

  auto write_raw_coord_mgf =
      std::make_shared<
      geom_view_io_saver_t<
      write_raw_coord<
      float, periodic_cell<float> >, false, 0 >
      >();

  auto vasp_outcar_mgf =
      std::make_shared<
      geom_view_io_anim_loader_t<
      read_vasp_outcar_md_with_frames<float, periodic_cell<float> >, 3 >
      >();

  auto generic_cube_mgf =
      std::make_shared<geom_view_io_cube_t
      >();

  auto generic_cube3d_mgf =
      std::make_shared<geom_view_io_cube_t
      >();

  generic_cube3d_mgf->m_cell_emplace = true;

  auto generic_molcas_grid_mgf =
      std::make_shared<geom_view_molcas_grid_t
      >();

  bhv_mgr->reg_io_bhv(xyz_ff_mgr, xyz_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(xyz_s_ff_mgr, xyz_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(xyzq_mgf, xyzq_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(xyzq_s_mgf, xyzq_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(ff_output_mgf, firefly_out_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(cp2k_output_mgf, cp2k_out_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(cp2k_cs_mgf, cp2k_cs_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(vasp_poscar_mgf, poscar_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(vasp_poscar_s_mgf, poscar_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(vasp_outcar_mgf, outcar_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(generic_cube_mgf, generic_cube_ff_hash, geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(generic_cube3d_mgf, generic_cube3d_ff_hash,
                      geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(generic_molcas_grid_mgf, generic_molcas_grid_ff_hash,
                      geom_view_t::get_type_static());
  bhv_mgr->reg_io_bhv(write_raw_coord_mgf, generic_raw_coord_ff_hash,
                      geom_view_t::get_type_static());

}

void registration_helper_t::reg_ws_item_tools(ws_item_behaviour_manager_t *bhv_mgr) {

  size_t hash_t_generator = bhv_mgr->reg_tool_grp("Generators");
  size_t hash_t_tr = bhv_mgr->reg_tool_grp("Modify geometry");
  size_t hash_t_generic = bhv_mgr->reg_tool_grp("Generic tools");

  registration_helper_t::reg_ws_item_tool<supercell_tool_t, geom_view_t>(
        "Supercell",
        hash_t_generator,
        bhv_mgr);

  registration_helper_t::reg_ws_item_tool<axial_scale_tool_t, geom_view_t>(
        "Axial scale",
        hash_t_tr,
        bhv_mgr);

  registration_helper_t::reg_ws_item_tool<clamp_atoms_to_cell_tool_t, geom_view_t>(
        "Clamp atoms to cell(3D)",
        hash_t_tr,
        bhv_mgr);

  registration_helper_t::reg_ws_item_tool<colorize_by_xfield_tool_t, geom_view_t>(
        "Colorize by xfield",
        hash_t_generic,
        bhv_mgr);

  registration_helper_t::reg_ws_item_tool<structure_similarity_tool_t, geom_view_t>(
        "Structure similarity",
        hash_t_generic,
        bhv_mgr,
        false);

}
