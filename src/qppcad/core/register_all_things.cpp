#include <qppcad/core/register_all_things.hpp>

#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/geom_view/geom_view_io.hpp>
#include <qppcad/ws_item/geom_view/geom_view_extended_editor.hpp>

#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/ccd_view/ccd_vib_analyzer_extended_editor.hpp>
#include <qppcad/ws_item/ccd_view/ccd_traj_analyzer_extended_editor.hpp>

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

#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/node_book/node_book_obj_insp_widget.hpp>
#include <qppcad/ws_item/node_book/node_book_extended_editor.hpp>

#include <qppcad/ws_item/py_note_book/py_note_book.hpp>
#include <qppcad/ws_item/py_note_book/py_note_book_obj_insp_widget.hpp>
#include <qppcad/ws_item/py_note_book/py_note_book_extended_editor.hpp>

#include <qppcad/ws_item/compl_list_view/compl_list_view.hpp>
#include <qppcad/ws_item/compl_list_view/compl_list_view_obj_insp_widget.hpp>

#include <qppcad/ws_item/ws_vector3/ws_vector3.hpp>
#include <qppcad/ws_item/ws_vector3/ws_vector3_obj_insp_widget.hpp>

#include <qppcad/ws_item/ws_matrix3/ws_matrix3.hpp>
#include <qppcad/ws_item/ws_matrix3/ws_matrix3_obj_insp_widget.hpp>

#include <qppcad/tools/supercell/supercell.hpp>
#include <qppcad/tools/axial_scale/axial_scale.hpp>
#include <qppcad/tools/clamp_atoms_to_cell/clamp_atoms_to_cell.hpp>
#include <qppcad/tools/colorize_by_xfield/colorize_by_xfield.hpp>
#include <qppcad/tools/colorize_by_dist/colorize_by_dist.hpp>
#include <qppcad/tools/structure_similarity/structure_similarity.hpp>
#include <qppcad/tools/nn_dist_msr/nn_dist_msr.hpp>
#include <qppcad/tools/anim_lerp/anim_lerp.hpp>
#include <qppcad/tools/center_cell_on_atom/center_cell_on_atom.hpp>
#include <qppcad/tools/compose_anim_from_files/compose_anim_from_files.hpp>
#include <qppcad/tools/purify_boundary_atoms/purify_boundary_atoms.hpp>
#include <qppcad/tools/copy_geom_view_aux/copy_geom_view_aux.hpp>
#include <qppcad/tools/shift_start_of_cell/shift_start_of_cell.hpp>
#include <qppcad/tools/sel_vis/sel_vis.hpp>
#include <qppcad/tools/sel_parity/sel_parity.hpp>
#include <qppcad/tools/geom_view_checkers.hpp>

#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>
#include <qppcad/ws_item/pgf_producer/pgf_producer_obj_insp_widget.hpp>

#include <qppcad/ws_item/arrow_array/arrow_array.hpp>
#include <qppcad/ws_item/arrow_array/arrow_array_obj_insp_widget.hpp>

#include <qppcad/ws_item/pdos_view/pdos_view.hpp>
#include <qppcad/ws_item/pdos_view/pdos_view_obj_insp_widget.hpp>
#include <qppcad/ws_item/pdos_view/pdos_view_extended_editor.hpp>

#include <io/ccd_molden.hpp>
#include <io/ccd_hoomd_xml.hpp>
#include <io/ccd_orca.hpp>
#include <io/write_coord.hpp>
#include <io/adme.hpp>

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
  reg_ws_item_fbr<node_book_t>(bhv_mgr);
  reg_ws_item_fbr<arrow_array_t>(bhv_mgr);
  reg_ws_item_fbr<py_note_book_t>(bhv_mgr);
  reg_ws_item_fbr<pdos_view_t>(bhv_mgr);
  reg_ws_item_fbr<compl_list_view_t>(bhv_mgr);
  reg_ws_item_fbr<ws_vector3_t>(bhv_mgr);
  reg_ws_item_fbr<ws_matrix3_t>(bhv_mgr);

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
  reg_ws_item_obj_insp_fbr<node_book_t, node_book_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<arrow_array_t, arrow_array_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<py_note_book_t, py_note_book_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<pdos_view_t, pdos_view_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<compl_list_view_t, compl_list_view_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<ws_vector3_t, ws_vector3_obj_insp_widget_t>(bhv_mgr);
  reg_ws_item_obj_insp_fbr<ws_matrix3_t, ws_matrix3_obj_insp_widget_t>(bhv_mgr);

}

void registration_helper_t::reg_ws_item_ext_edt(ws_item_behaviour_manager_t *bhv_mgr) {

  reg_ws_item_ext_edt_fbr<node_book_t, node_book_extended_editor_t>(
        0, "node book editor", bhv_mgr);

  reg_ws_item_ext_edt_fbr<geom_view_t, geom_view_extended_editor_t>(
        0, "xgeometry editor", bhv_mgr);

  reg_ws_item_ext_edt_fbr<ccd_view_t, ccd_vib_analyzer_extended_editor_t>(
        0, "ccd vib_analyzer", bhv_mgr);

  reg_ws_item_ext_edt_fbr<ccd_view_t, ccd_traj_analyzer_extended_editor_t>(
        1, "ccd traj_analyzer", bhv_mgr);

  reg_ws_item_ext_edt_fbr<py_note_book_t, py_note_book_extended_editor_t>(
        0, "py_note_book editor", bhv_mgr);

  reg_ws_item_ext_edt_fbr<pdos_view_t, pdos_view_extended_editor_t>(
        0, "pdos viewer", bhv_mgr);

}

void registration_helper_t::reg_ws_item_io_bhv(ws_item_behaviour_manager_t *bhv_mgr) {

  /* ff hashes */

  size_t xyz_ff_g_hash = bhv_mgr->reg_ffg("XYZ", "xyz");
  size_t vasp_ff_g_hash = bhv_mgr->reg_ffg("VASP", "vasp");
  size_t firefly_ff_g_hash = bhv_mgr->reg_ffg("Firefly", "ff");
  size_t cp2k_ff_g_hash = bhv_mgr->reg_ffg("CP2K", "cp2k");
  size_t orca_ff_g_hash = bhv_mgr->reg_ffg("Orca", "orca");
  size_t generic_ff_g_hash = bhv_mgr->reg_ffg("Generic formats", "generic");

  size_t xyz_ff_hash =
      bhv_mgr->reg_ff("Standart XYZ", "xyz", xyz_ff_g_hash, {".xyz"});

  size_t xyzq_ff_hash =
      bhv_mgr->reg_ff("UC(XYZQ)", "uc", xyz_ff_g_hash, {".uc"});

  size_t poscar_ff_hash =
      bhv_mgr->reg_ff("VASP POSCAR[CONTCAR]", "poscar", vasp_ff_g_hash,
      {"POSCAR", ".vasp", ".VASP", "CONTCAR"} );

  size_t chgcar_ff_hash =
      bhv_mgr->reg_ff("VASP CHGCAR", "chgcar", vasp_ff_g_hash, {"PARCHG", "CHGCAR", "chgcar"} );

  size_t outcar_ff_hash =
      bhv_mgr->reg_ff("VASP OUTCAR", "outcar", vasp_ff_g_hash, {"OUTCAR"} );

  size_t firefly_out_ff_hash =
      bhv_mgr->reg_ff("FF Output", "ffout", firefly_ff_g_hash, {".out", ".ff"} );

  size_t cp2k_out_ff_hash =
      bhv_mgr->reg_ff("CP2K Output", "cp2k", cp2k_ff_g_hash, {"cp2k", ".cout"} );

  size_t cp2k_cs_ff_hash =
      bhv_mgr->reg_ff("CP2K Crd. Section", "cp2kcs", cp2k_ff_g_hash, {".coord", ".cp2k_crd"} );

  size_t generic_cube_ff_hash =
      bhv_mgr->reg_ff("CUBE File", "cube", generic_ff_g_hash, {".cube", ".CUBE"} );

  size_t generic_cube3d_ff_hash =
      bhv_mgr->reg_ff("CUBE File(3d)", "cube3d", generic_ff_g_hash, {".cube", ".CUBE"} );

  size_t generic_molcas_grid_ff_hash =
      bhv_mgr->reg_ff("Molcas ASCII Grid", "gv", generic_ff_g_hash, {".grid", ".GRID"} );

  size_t generic_raw_coord_ff_hash =
      bhv_mgr->reg_ff("Simple Crd.", "coord", generic_ff_g_hash, {"coord", "coord"} );

  size_t generic_atoms_coord_ff_hash =
      bhv_mgr->reg_ff("Simple Crd.[name]", "coord", generic_ff_g_hash, {"coord", "coord"} );

  size_t generic_atoms_coord_chg_ff_hash =
      bhv_mgr->reg_ff("Simple Crd.[name, chg.]", "coord", generic_ff_g_hash, {"coord", "coord"} );

  size_t molden_ff_hash =
      bhv_mgr->reg_ff("Molden", "molden", generic_ff_g_hash, {"mol", "molden"} );

  size_t hoomd_xml_ff_hash =
      bhv_mgr->reg_ff("Hoomd Xml", "hoomd_xml", generic_ff_g_hash, {"hoomd", "xml"} );

  size_t adme_ewald_uc_ff_hash =
      bhv_mgr->reg_ff("Adme Ewald Cell", "adme_ewald", generic_ff_g_hash, {"adme_ewald"} );

  size_t orca_output_ff_hash =
      bhv_mgr->reg_ff("Orca Output", "orca", orca_ff_g_hash, {"orca.out"} );

  /* ws_item_t io mgrs */
  auto xyz_ff_mgr =
      std::make_shared<
      geom_view_io_ccd_t<
      read_ccd_from_xyz_file<float>, true, true, true, false, false >
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

  auto vasp_chgcar_mgf = std::make_shared<geom_view_vasp_chgcar_t>();

  auto write_raw_coord_mgf =
      std::make_shared<
      geom_view_io_saver_t<write_raw_coord<float, periodic_cell<float> >, false, 0 > >();

  auto write_adme_ewald_uc_mgf =
      std::make_shared<
      geom_view_io_saver_t<write_adme_ewald_uc<float, periodic_cell<float> >, false, 0 > >();

  auto write_atoms_with_coord_mgf =
      std::make_shared<
      geom_view_io_saver_t<write_atoms_with_coord<float, periodic_cell<float> >, false, 0 > >();

  auto write_atoms_with_coord_and_chg_mgf =
      std::make_shared<
      geom_view_io_saver_t<write_atoms_with_coord_and_chg<float, periodic_cell<float> >,
      false, 0 > >();

  auto vasp_outcar_mgf =
      std::make_shared<
      geom_view_io_anim_loader_t<
      read_vasp_outcar_md_with_frames<float, periodic_cell<float> >, 3 >
      >();

  auto generic_cube_mgf = std::make_shared<geom_view_io_cube_t>();

  auto molden_mgf =
      std::make_shared<
      geom_view_io_ccd_t<read_ccd_from_molden<float>,  true, true, true, false, false, 0> >();

  auto hoomd_xml_mgf =
      std::make_shared<
      geom_view_io_ccd_t<read_ccd_from_hoomd_xml<float>,  true, true, true, false, false, 3> >();

  auto generic_cube3d_mgf = std::make_shared<geom_view_io_cube_t>();

  generic_cube3d_mgf->m_cell_emplace = true;

  auto generic_molcas_grid_mgf = std::make_shared<geom_view_molcas_grid_t>();

  auto orca_output_mgf =
      std::make_shared<
      geom_view_io_ccd_t<read_ccd_from_orca_output<float>,  true, true, true, false, false, 0> >();

  /* register ws_item io bhv */

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
  bhv_mgr->reg_io_bhv(vasp_chgcar_mgf, chgcar_ff_hash, geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(generic_cube_mgf, generic_cube_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(generic_cube3d_mgf, generic_cube3d_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(generic_molcas_grid_mgf, generic_molcas_grid_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(write_raw_coord_mgf, generic_raw_coord_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(write_atoms_with_coord_mgf, generic_atoms_coord_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(write_atoms_with_coord_and_chg_mgf, generic_atoms_coord_chg_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(write_adme_ewald_uc_mgf, adme_ewald_uc_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(molden_mgf, molden_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(hoomd_xml_mgf, hoomd_xml_ff_hash,
                      geom_view_t::get_type_static());

  bhv_mgr->reg_io_bhv(orca_output_mgf, orca_output_ff_hash,
                      geom_view_t::get_type_static());

}

void registration_helper_t::reg_toolbar_elements_bhv(ws_item_behaviour_manager_t *bhv_mgr) {

}

void registration_helper_t::reg_ws_item_tools(ws_item_behaviour_manager_t *bhv_mgr) {

  size_t hash_t_generator = bhv_mgr->reg_tool_grp("Generators");
  size_t hash_t_colorize = bhv_mgr->reg_tool_grp("Colorize");
  size_t hash_t_tr = bhv_mgr->reg_tool_grp("Modify geometry");
  size_t hash_t_generic = bhv_mgr->reg_tool_grp("Generic tools");
  size_t hash_t_selection = bhv_mgr->reg_tool_grp("Selection");

  registration_helper_t::reg_ws_item_tool<supercell_tool_t, geom_view_t>(
        "Supercell Generator", hash_t_generator, bhv_mgr,
        true, ws_item_tool_type_e::ws_item_tool_inline_vertical,
        gv_accept_dim<false, false, false, true>);

  registration_helper_t::reg_ws_item_tool<axial_scale_tool_t, geom_view_t>(
        "Axial scale", hash_t_tr, bhv_mgr,
        true, ws_item_tool_type_e::ws_item_tool_modal,
        gv_accept_dim<false, false, false, true>);

  registration_helper_t::reg_ws_item_tool<clamp_atoms_to_cell_tool_t, geom_view_t>(
        "Clamp atoms to cell(3D)", hash_t_tr, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<colorize_by_xfield_tool_t, geom_view_t>(
        "Colorize atoms by xfield", hash_t_colorize, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<colorize_by_dist_tool_t, geom_view_t>(
        "Colorize atoms by dist", hash_t_colorize, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<structure_similarity_tool_t, geom_view_t>(
        "Structure similarity", hash_t_generic, bhv_mgr, false);

  registration_helper_t::reg_ws_item_tool<nn_dist_msr_tool_t, geom_view_t>(
        "Nearest neighbour distance msr.", hash_t_generic, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<anim_lerp_tool_t, geom_view_t>(
        "Create interpolated animation", hash_t_generic, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<center_cell_on_atom_tool_t, geom_view_t>(
        "Shift center of cell to atoms", hash_t_tr, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<shift_start_of_cell_tool_t, geom_view_t>(
        "Shift start of cell to atoms", hash_t_tr, bhv_mgr);

  registration_helper_t::reg_ws_item_tool<compose_anim_tool_t, geom_view_t>(
        "Compose animation", hash_t_generic, bhv_mgr, false);

  registration_helper_t::reg_ws_item_tool<purify_boundary_atoms_tool_t, geom_view_t>(
        "Purify boundary atoms", hash_t_tr, bhv_mgr, true);

  registration_helper_t::reg_ws_item_tool<copy_geom_view_aux_tool_t, geom_view_t>(
        "Copy geom. view data", hash_t_tr, bhv_mgr, true);

  registration_helper_t::reg_ws_item_tool<sel_vis_tool_t, geom_view_t>(
        "Select visible atoms", hash_t_selection, bhv_mgr, true);

  registration_helper_t::reg_ws_item_tool<sel_parity_tool_t, geom_view_t>(
        "Select atoms by parity", hash_t_selection, bhv_mgr, true);

}
