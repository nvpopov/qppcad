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
        static std::optional<std::tuple<size_t, size_t> >
        extract_state_spin_subspace_from_name(const std::string &file_name);

    };

    /**
     * @brief The ws_item_io_bt_bhv_t class
     */
    class ws_item_io_bt_bhv_t : public ws_item_io_inherited_bhv_hooked_t<geom_view_t> {

      public:

        bool can_save() override { return true; }
        bool can_load() override { return false; }

         void pre_load_hook(geom_view_t *_item, workspace_t *ws) override {

          _item->begin_structure_change();

        }

         void post_load_hook(geom_view_t *_item, workspace_t *ws) override {

          if (_item->m_geom->nat() > 20000) {
              _item->m_render_style = geom_view_render_style_e::billboards;
            }
          else if (_item->m_geom->nat() > 7000) {
              _item->m_draw_bonds = false;
              _item->m_draw_img_bonds = false;
            }

          _item->end_structure_change() ;

        }

        void pre_save_hook(geom_view_t *_item) override {}
        void post_save_hook(geom_view_t *_item) override {}

    };

    template<auto GEN_FUNC_GEOM, bool CHECK_DIM = false, int REQUIRED_DIM = -1>
    class geom_view_io_saver_t : public ws_item_io_inherited_bhv_t<geom_view_t> {

      public:

        bool can_save() override { return true; }
        bool can_load() override { return false; }

        bool check_before_save_ex(geom_view_t *_item, std::string &message) {

          if (CHECK_DIM) {
              bool check = _item->m_geom->DIM == REQUIRED_DIM;
              if (!check)
                message = fmt::format("Invalid dimension -> REQUIRED = {}, FOUND = {}",
                                      REQUIRED_DIM, _item->m_geom->DIM);
              return check;
            } else {
              return true;
            }

        }

        void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                 geom_view_t *_item,
                                 workspace_t *ws) override {
        }

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {
          GEN_FUNC_GEOM(stream, *(_item->m_geom.get()));
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
                                 geom_view_t *_item,
                                 workspace_t *ws) override {

          if (FORCED_DIM != -1) {
              _item->m_geom->DIM = FORCED_DIM;
              _item->m_geom->cell.DIM = FORCED_DIM;
            }

          GEN_FUNC_GEOM(stream, *(_item->m_geom.get()));

          _item->m_role = ROLE;

        }

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {
          //do nothing
        }

    };

    template<auto GENERIC_FUNC_GEOM_ANIM, int FORCED_DIM = -1>
    class geom_view_io_anim_loader_t : public ws_item_io_bt_bhv_t {

      public:

        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                 geom_view_t *_item,
                                 workspace_t *ws) override {

          if (FORCED_DIM != -1) {
              _item->m_geom->DIM = FORCED_DIM;
              _item->m_geom->cell.DIM = FORCED_DIM;
            }

          GENERIC_FUNC_GEOM_ANIM(stream, *(_item->m_geom.get()), _item->m_anim->m_anim_data);

        }

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {
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
    class geom_view_io_ccd_t : public ws_item_io_bt_bhv_t {

      public:

        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                 geom_view_t *_item,
                                 workspace_t *ws) override {

          app_state_t* astate = app_state_t::get_inst();

          comp_chem_program_data_t<float> cc_inst;
          CCD_FUNC(stream, cc_inst);

          if (FORCED_DIM != -1) {
              _item->m_geom->DIM = FORCED_DIM;
              _item->m_geom->cell.DIM = FORCED_DIM;
            }

          if (COMPILE_CCD) {
              bool succes_comp_ccd = compile_ccd(cc_inst, ccd_cf_default_flags |
                                                 ccd_cf_remove_empty_geom_steps);
              astate->tlog("Is ccd compilation succes? {}", succes_comp_ccd);
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

              astate->tlog("Is geometry compilation succes? {}",
                           succes_comp_geom && succes_comp_static_anim);

              if (_item->m_anim->get_total_anims() > 1 && succes_anims)
                astate->tlog("Animations have been added to geom");

            }

          if (EXTRACT_CCD) {

              std::shared_ptr<ccd_view_t> extracted_ccd =
                  std::make_shared<ccd_view_t>();
              extracted_ccd->m_name = _item->m_name+"_ccd";
              extracted_ccd->m_ccd =
                  std::make_shared<comp_chem_program_data_t<float> >(std::move(cc_inst));
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

        }

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {
          //do nothing
        }

    };


    class geom_view_io_cube_t : public ws_item_io_bt_bhv_t {

      public:

        bool m_cell_emplace{false};
        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                 geom_view_t *_item,
                                 workspace_t *ws) override ;

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {
          //do nothing
        }

    }; // class geom_view_io_cube_t


    class geom_view_molcas_grid_t : public ws_item_io_bt_bhv_t {

      public:

        bool m_cell_emplace{false};
        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                 geom_view_t *_item,
                                 workspace_t *ws) override ;

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {}

    }; // class geom_view_molcas_grid_t

    class geom_view_vasp_chgcar_t : public ws_item_io_bt_bhv_t {

      public:

        bool can_save() override { return false; }
        bool can_load() override { return true; }

        void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                 geom_view_t *_item,
                                 workspace_t *ws) override ;

        void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                               geom_view_t *_item) override {}

    }; // class geom_view_molcas_grid_t

  } // namespace qpp::cad

} // namespace qpp

#endif
