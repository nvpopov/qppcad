#ifndef QPPCAD_WS_ATOMS_LIST_IO_XYZ
#define QPPCAD_WS_ATOMS_LIST_IO_XYZ

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/core/app_state.hpp>
#include <io/ccd_firefly.hpp>
#include <io/ccd_xyz.hpp>
#include <io/ccd_cp2k.hpp>
#include <io/geomio.hpp>
#include <io/vasp_io.hpp>
#include <io/cp2k.hpp>
#include <io/xyz_multiframe.hpp>
#include <io/cube.hpp>

namespace qpp {

namespace cad {

class geom_view_io_helpers_t {
public :
  static std::optional<std::tuple<size_t, size_t>>
  extract_state_spin_subspace_from_name(const std::string &file_name);
};

/**
 * @brief The wsitem_io_bt_bhv_t class
 */
class ws_item_io_bt_bhv_t : public ws_item_io_inherited_bhv_hooked_t<geom_view_t> {
public:
  bool can_save() override { return true; }
  bool can_load() override { return false; }

  void pre_load_hook(geom_view_t *item, workspace_t *ws) override {

    item->begin_structure_change();
    item->m_xgeom_proxy.set_ignore_changes(true);

  }

  void post_load_hook(geom_view_t *item, workspace_t *ws) override {

    if (item->m_geom->nat() > 20000) {
      item->m_render_style.set_value(geom_view_render_style_e::billboards);
    } else if (item->m_geom->nat() > 7000) {
      item->m_draw_bonds.set_value(false);
      item->m_draw_img_bonds.set_value(false);
    }

    item->end_structure_change() ;
    item->m_xgeom_proxy.set_ignore_changes(false);
    //moved to end_recording
    //item->m_xgeom_proxy.init_base_epoch();

  }

  void pre_save_hook(geom_view_t *item) override {}
  void post_save_hook(geom_view_t *item) override {}
};

template<auto GEN_FUNC_GEOM, bool CHECK_DIM = false, int REQUIRED_DIM = -1>
class geom_view_io_saver_t : public ws_item_io_inherited_bhv_t<geom_view_t> {
public:
  bool can_save() override { return true; }
  bool can_load() override { return false; }

  bool check_before_save_ex(geom_view_t *item, std::string &message) override {
    if (CHECK_DIM) {
      bool check = item->m_geom->get_DIM() == REQUIRED_DIM;
      if (!check)
        message = fmt::format("Invalid dimension -> REQUIRED = {}, FOUND = {}",
                              REQUIRED_DIM, item->m_geom->get_DIM());
      return check;
    } else {
      return true;
    }
  }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override {
  }

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {
    GEN_FUNC_GEOM(stream, *(item->m_geom.get()));
  }
};

template<auto GEN_FUNC_GEOM,
          geom_view_role_e ROLE = geom_view_role_e::r_generic,
          int FORCED_DIM = -1>
class geom_view_io_loader_t : public ws_item_io_bt_bhv_t {
public:
  bool can_save() override { return false; }
  bool can_load() override { return true; }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override {
    if (FORCED_DIM != -1) {
      item->m_geom->set_DIM(FORCED_DIM);
      //item->m_geom->cell.DIM = FORCED_DIM;
    }
    GEN_FUNC_GEOM(stream, *(item->m_geom.get()));
    item->m_role.set_value(ROLE);
  }

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {
    //do nothing
  }
};

template<auto GENERIC_FUNC_GEOM_ANIM, int FORCED_DIM = -1>
class geom_view_io_anim_loader_t : public ws_item_io_bt_bhv_t {
public:
  bool can_save() override { return false; }
  bool can_load() override { return true; }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override {
    if (FORCED_DIM != -1) {
      item->m_geom->set_DIM(FORCED_DIM);
      //item->m_geom->cell.DIM = FORCED_DIM;
    }
    GENERIC_FUNC_GEOM_ANIM(stream, *(item->m_geom.get()), item->m_anim->m_anim_data);
  }

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {
    //do nothing
  }
};

template<auto CCD_FUNC, bool COMPILE_CCD = false, bool EXTRACT_CCD = false,
         bool COMPILE_FROM_CCD = false, bool AUTO_CENTER = false,
         bool COPY_DIM_FROM_CCD = false, int FORCED_DIM = -1>
class geom_view_io_ccd_t : public ws_item_io_bt_bhv_t {
public:
  bool can_save() override { return false; }
  bool can_load() override { return true; }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override {
    app_state_t* astate = app_state_t::get_inst();
    comp_chem_program_data_t<float> cc_inst;
    CCD_FUNC(stream, cc_inst);
    if (FORCED_DIM != -1) {
      item->m_geom->set_DIM(FORCED_DIM);
      //item->m_geom->cell.DIM = FORCED_DIM;
    }

    if (COMPILE_CCD) {
      bool succes_comp_ccd =
          compile_ccd(cc_inst, ccd_cf_default_flags | ccd_cf_remove_empty_geom_steps);
      astate->tlog("[GEOM_VIEW_IO] Is ccd compilation succes? {}", succes_comp_ccd);
    }

    if (COPY_DIM_FROM_CCD) {
      item->m_geom->set_DIM(cc_inst.m_DIM);
      //item->m_geom->cell.DIM = cc_inst.m_DIM;
    }

    if (COMPILE_FROM_CCD) {
      bool succes_comp_geom = compile_geometry(cc_inst, *(item->m_geom.get()));
      bool succes_comp_static_anim =
          compile_static_animation(cc_inst, item->m_anim->m_anim_data);
      bool succes_anims = compile_animation(cc_inst, item->m_anim->m_anim_data);
      astate->tlog("[GEOM_VIEW_IO] Is geometry compilation succes? {}",
                   succes_comp_geom && succes_comp_static_anim);
      if (item->m_anim->get_total_anims() > 1 && succes_anims) {
        astate->tlog("[GEOM_VIEW_IO] Animations have been added to geom");
        item->m_anim->make_animable();
      }
    }

    if (EXTRACT_CCD) {
      std::shared_ptr<ccd_view_t> ex_ccd = std::make_shared<ccd_view_t>();
      ex_ccd->m_name.set_value(item->m_name.get_value() + "_ccd");
      ex_ccd->m_ccd = std::make_shared<comp_chem_program_data_t<float>>(std::move(cc_inst));
      ex_ccd->m_connected_items.push_back(item->shared_from_this());
      ex_ccd->m_connected_items_stride.push_back(0);
      item->m_parent_ws->add_item_to_ws(ex_ccd);
    }

    if (AUTO_CENTER) {
      vector3<float> center(0.0, 0.0, 0.0);
      for (int i = 0; i < item->m_geom->nat(); i++)
        center += item->m_geom->pos(i);
      center *= (1.0f / item->m_geom->nat());
      for (int i = 0; i < item->m_geom->nat(); i++)
        item->m_geom->coord(i) = -center + item->m_geom->pos(i) ;
      item->m_ext_obs->aabb.min = -center + item->m_ext_obs->aabb.min;
      item->m_ext_obs->aabb.max = -center + item->m_ext_obs->aabb.max;
      for (auto &anim : item->m_anim->m_anim_data)
        for (auto &anim_frame : anim.frames)
          for (auto &anim_frame_rec : anim_frame.atom_pos)
            anim_frame_rec -= center;
    }
  }

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {
    //do nothing
  }
};

class geom_view_io_cube_t : public ws_item_io_bt_bhv_t {
public:
  bool m_cell_emplace{false};
  bool can_save() override { return false; }
  bool can_load() override { return true; }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override ;

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {
    //do nothing
  }
}; // class geom_view_io_cube_t

class geom_view_molcas_grid_t : public ws_item_io_bt_bhv_t {
public:
  bool m_cell_emplace{false};
  bool can_save() override { return false; }
  bool can_load() override { return true; }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override ;

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {}
}; // class geom_view_molcas_grid_t

class geom_view_vasp_chgcar_t : public ws_item_io_bt_bhv_t {
public:
  bool can_save() override { return false; }
  bool can_load() override { return true; }

  void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                           geom_view_t *item,
                           workspace_t *ws) override ;

  void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                         geom_view_t *item) override {}
}; // class geom_view_molcas_grid_t

} // namespace qpp::cad

} // namespace qpp

#endif
