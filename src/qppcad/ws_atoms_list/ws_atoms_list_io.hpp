#ifndef QPP_CAD_WS_ATOMS_LIST_IO_XYZ
#define QPP_CAD_WS_ATOMS_LIST_IO_XYZ

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_comp_chem_data/ws_comp_chem_data.hpp>
#include <qppcad/ws_item_behaviour_manager.hpp>
#include <qppcad/app_state.hpp>
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

    template<auto GENERIC_FUNC_GEOM,
             int FORCED_DIM = -1>
    class ws_atoms_list_io_loader_t : public ws_item_io_inherited_bhv_t<ws_atoms_list_t> {

      public:

        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR,TRAITS> &stream,
                                 ws_atoms_list_t *_item,
                                 workspace_t *ws) override {

          if (FORCED_DIM != -1) {
              _item->m_geom->DIM = FORCED_DIM;
              _item->m_geom->cell.DIM = FORCED_DIM;
            }

          _item->m_tws_tr->do_action(act_lock | act_clear_all);
          _item->m_ext_obs->first_data = true;

          GENERIC_FUNC_GEOM(stream, *(_item->m_geom.get()));


          if (_item->m_geom->nat() > 20000) {
              _item->m_cur_render_type = ws_atoms_list_render_t::billboards;
            }
          else if (_item->m_geom->nat() > 7000) {
              _item->m_draw_bonds = false;
              _item->m_draw_img_bonds = false;
            }

          _item->geometry_changed();

          _item->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
          _item->m_tws_tr->do_action(act_rebuild_ntable);
        }

        void save_to_stream_ex(std::basic_ostream<CHAR,TRAITS> &stream,
                               ws_atoms_list_t *_item) {
          //do nothing
        }

    };

    template<auto GENERIC_FUNC_GEOM_ANIM,
             int FORCED_DIM = -1>
    class ws_atoms_list_io_anim_loader_t :
        public ws_item_io_inherited_bhv_t<ws_atoms_list_t> {

      public:

        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR,TRAITS> &stream,
                                 ws_atoms_list_t *_item,
                                 workspace_t *ws) override {

          _item->m_tws_tr->do_action(act_lock | act_clear_all);
          _item->m_ext_obs->first_data = true;

          if (FORCED_DIM != -1) {
              _item->m_geom->DIM = FORCED_DIM;
              _item->m_geom->cell.DIM = FORCED_DIM;
            }

          GENERIC_FUNC_GEOM_ANIM(stream, *(_item->m_geom.get()), _item->m_anim->m_anim_data);

          if (_item->m_geom->nat() > 20000) {
              _item->m_cur_render_type = ws_atoms_list_render_t::billboards;
            }
          else if (_item->m_geom->nat() > 7000) {
              _item->m_draw_bonds = false;
              _item->m_draw_img_bonds = false;
            }

          _item->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
          _item->m_tws_tr->do_action(act_rebuild_ntable);

          _item->geometry_changed();
        }

        void save_to_stream_ex(std::basic_ostream<CHAR,TRAITS> &stream,
                               ws_atoms_list_t *_item) {
          //do nothing
        }

    };

    template<auto CCD_FUNC,
             bool COMPILE_CCD = false,
             bool EXTRACT_CCD = false,
             bool COMPILE_FROM_CCD = false,
             bool AUTO_CENTER = false,
             bool COPY_DIM_FROM_CCD = false,
             int FORCED_DIM = -1>
    class ws_atoms_list_io_ccd_t : public ws_item_io_inherited_bhv_t<ws_atoms_list_t> {

      public:

        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR,TRAITS> &stream,
                                 ws_atoms_list_t *_item,
                                 workspace_t *ws) override {

          app_state_t* astate = app_state_t::get_inst();

          _item->m_tws_tr->do_action(act_lock | act_clear_all);
          _item->m_ext_obs->first_data = true;

          comp_chem_program_data_t<float> cc_inst;
          CCD_FUNC(stream, cc_inst);

          if (FORCED_DIM != -1) {
              _item->m_geom->DIM = FORCED_DIM;
              _item->m_geom->cell.DIM = FORCED_DIM;
            }

          if (COMPILE_CCD) {
              bool succes_comp_ccd = compile_ccd(cc_inst, ccd_cf_default_flags |
                                                 ccd_cf_remove_empty_geom_steps);
              astate->log(fmt::format("Is ccd compilation succes? {}", succes_comp_ccd));
            }

          if (COPY_DIM_FROM_CCD) {
              _item->m_geom->DIM = cc_inst.m_DIM;
              _item->m_geom->cell.DIM = cc_inst.m_DIM;
            }

          if (COMPILE_FROM_CCD) {
              bool succes_comp_geom = compile_geometry(cc_inst, *(_item->m_geom.get()));
              bool succes_comp_static_anim =
                  compile_static_animation(cc_inst, _item->m_anim->m_anim_data);
              bool succes_anims = compile_animation(cc_inst, _item->m_anim->m_anim_data);

              astate->log(fmt::format("Is geometry compilation succes? {}",
                                      succes_comp_geom && succes_comp_static_anim));
              if (_item->m_anim->get_total_anims() > 1 && succes_anims)
                astate->log("Animations have been added to geom");
            }

          if (EXTRACT_CCD) {
              std::shared_ptr<ws_comp_chem_data_t> extracted_ccd =
                  std::make_shared<ws_comp_chem_data_t>();
              extracted_ccd->m_name = _item->m_name+"_ccd";
              extracted_ccd->m_ccd =
                  std::make_unique<comp_chem_program_data_t<float> >(std::move(cc_inst));
              extracted_ccd->m_connected_items.push_back(_item->shared_from_this());
              extracted_ccd->m_connected_items_stride.push_back(0);
              _item->m_parent_ws->add_item_to_ws(extracted_ccd);
            }

          if (AUTO_CENTER) {
              vector3<float> center(0.0, 0.0, 0.0);
              for (int i = 0; i < _item->m_geom->nat(); i++)
                center += _item->m_geom->pos(i);
              center *= (1.0f / _item->m_geom->nat());
              for (int i = 0; i < _item->m_geom->nat(); i++)
                _item->m_geom->coord(i) = -center + _item->m_geom->pos(i) ;

              _item->m_ext_obs->aabb.min = -center + _item->m_ext_obs->aabb.min;
              _item->m_ext_obs->aabb.max = -center + _item->m_ext_obs->aabb.max;

              for (auto &anim : _item->m_anim->m_anim_data)
                for (auto &anim_frame : anim.frames)
                  for (auto &anim_frame_rec : anim_frame.atom_pos)
                    anim_frame_rec -= center;
            }

          if (_item->m_geom->nat() > 20000) {
              _item->m_cur_render_type = ws_atoms_list_render_t::billboards;
            }
          else if (_item->m_geom->nat() > 7000) {
              _item->m_draw_bonds = false;
              _item->m_draw_img_bonds = false;
            }

          _item->m_tws_tr->do_action(act_unlock | act_rebuild_tree);
          _item->m_tws_tr->do_action(act_rebuild_ntable);

          _item->geometry_changed();

        }

        void save_to_stream_ex(std::basic_ostream<CHAR,TRAITS> &stream,
                               ws_atoms_list_t *_item) {
          //do nothing
        }

    };

  }

}

#endif
