#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/geom_view/geom_view_anim_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools_colorize.hpp>
#include <qppcad/ws_item/geom_view/geom_view_tools.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void geom_view_colorizer_helper::colorize_by_distance(geom_view_t *al,
                                                      float min_dist,
                                                      vector3<float> min_dist_color,
                                                      vector3<float> over_dist_color,
                                                      bool fill_def_color_on_miss,
                                                      bool affect_pairs,
                                                      std::string atom_type1,
                                                      std::string atom_type2) {

  if (!al) {
      throw std::runtime_error("colorize_by_distance -> !al");
    }

  //populate static anim
  if (al->m_anim->get_total_anims() == 0) {

      geom_anim_record_t<float> anim;

      anim.m_anim_type = geom_anim_e::anim_static;
      anim.m_anim_name = "static";
      anim.frames.resize(1);
      anim.frames[0].atom_pos.resize(al->m_geom->nat());
      for (size_t i = 0; i < al->m_geom->nat(); i++)
        anim.frames[0].atom_pos[i] = al->m_geom->pos(i);

      al->m_anim->m_anim_data.push_back(std::move(anim));

    }

  //intermediate geometry
  xgeometry<float, periodic_cell<float> > g(al->m_geom->get_DIM());

  g.set_DIM(al->m_geom->get_DIM());
  //g.cell.DIM = al->m_geom->cell.DIM;

  //initialize tws_tree
  tws_tree_t<float, periodic_cell<float> > g_t(g);
  g_t.do_action(act_lock | act_lock_img);

  index zero = index::D(al->m_geom->get_DIM()).all(0);

  //copy initial geometry
  al->copy_to_xgeom(g);

  //iterate over all animations
  for (size_t a_id = 0; a_id < al->m_anim->get_total_anims(); a_id++)
    //iterate over all frames
    for (size_t f_id = 0; f_id < al->m_anim->m_anim_data[a_id].frames.size(); f_id++) {
        //std::cout << "PROCCESING FRAME " << f_id << " FOR ANIM " << a_id << std::endl;
        //copy anim to temp geometry
        for (int i = 0; i < al->m_geom->nat(); i++)
          g.coord(i) = al->m_anim->m_anim_data[a_id].frames[f_id].atom_pos[i];

        g_t.do_action(act_unlock | act_unlock_img);
        g_t.do_action(act_clear_all);
        g_t.do_action(act_build_tree);
        g_t.do_action(act_lock | act_lock_img);

        if (al->m_anim->m_anim_data[a_id].frames[f_id].atom_colors.size() != al->m_geom->nat())
          al->m_anim->m_anim_data[a_id].frames[f_id].atom_colors.resize(al->m_geom->nat());

        for (int i = 0; i < g.nat(); i++) {

            std::vector<tws_node_cnt_t<float> > res;
            g_t.query_sphere(min_dist + 0.05f, g.coord(i), res);

            vector3<float> final_color = over_dist_color;

            if (fill_def_color_on_miss) {
                auto ap_idx1 = ptable::number_by_symbol(al->m_geom->atom(i));
                if (ap_idx1) final_color = ptable::get_inst()->arecs[*ap_idx1 - 1].m_color_jmol;
              }

            int first_n = i;
            int second_n = i;

            for (auto &elem : res)
              if (elem.m_idx == zero && elem.m_atm != size_t(i)) second_n = elem.m_atm;

            if (first_n != second_n && res.size() > 1) {

                bool pair1_d = al->m_geom->atom(first_n) == atom_type1;
                bool pair2_d = al->m_geom->atom(second_n) == atom_type2;

                bool pair1_i = al->m_geom->atom(first_n) == atom_type2;
                bool pair2_i = al->m_geom->atom(second_n) == atom_type1;

                bool p1 = pair1_d && pair2_d;
                bool p2 = pair1_i && pair2_i;

                if (!affect_pairs || p1 || p2) final_color = min_dist_color;

              }

            al->m_anim->m_anim_data[a_id].frames[f_id].atom_colors[i] = final_color;

          }

      }

  al->load_color_from_static_anim();

}

void geom_view_colorizer_helper::py_colorize_by_distance(float min_dist,
                                                         vector3<float> min_dist_color,
                                                         vector3<float> over_dist_color) {
  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

      auto cur_ws = astate->ws_mgr->get_cur_ws();

      if (cur_ws) {
          //
          std::shared_ptr<geom_view_t> as_al =
              std::dynamic_pointer_cast<geom_view_t>(cur_ws->get_sel_sp());

          if (as_al) geom_view_colorizer_helper::colorize_by_distance(
                as_al.get(), min_dist, min_dist_color, over_dist_color, true, false, "", "");
          else {
              throw std::runtime_error("colorize_by_distance -> !al");
            }
        }
    }
}

void geom_view_colorizer_helper::py_colorize_by_distance_with_pairs(
    float min_dist,
    vector3<float> min_dist_color,
    vector3<float> over_dist_color,
    std::string atom_type1,
    std::string atom_type2) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_mgr->has_wss()) {

      auto cur_ws = astate->ws_mgr->get_cur_ws();

      if (cur_ws) {
          //
          std::shared_ptr<geom_view_t> as_al =
              std::dynamic_pointer_cast<geom_view_t>(cur_ws->get_sel_sp());

          if (as_al) geom_view_colorizer_helper::colorize_by_distance(
                as_al.get(), min_dist, min_dist_color, over_dist_color,
                true, true, atom_type1, atom_type2);
          else {
              throw std::runtime_error("colorize_by_distance -> !al");
            }
        }

    }

}

void geom_view_colorizer_helper::colorize_by_xfield(geom_view_t *gv,
                                                    const vector3<float> color_low,
                                                    const vector3<float> color_high,
                                                    const size_t xfield_id) {

  if (!gv) return;

  auto field_min_max = gv->get_min_max_xfield(xfield_id);

  //  bool has_static_anim{false};
  std::optional<size_t> static_anim_id;

  for (size_t i = 0; i < gv->m_geom->nat(); i++) {

    float el_val = std::get<1>(field_min_max) - gv->m_geom->xfield<float>(xfield_id, i);
    float len = std::get<1>(field_min_max) - std::get<0>(field_min_max);
    vector3<float> color_interp = color_low + (color_high - color_low) * (1 - el_val / len);

    gv->m_geom->xfield<float>(xg_ccr, i) = color_interp[0];
    gv->m_geom->xfield<float>(xg_ccg, i) = color_interp[1];
    gv->m_geom->xfield<float>(xg_ccb, i) = color_interp[2];

    //TODO: propagate colors to first static anim

  }

}

void geom_view_colorizer_helper::colorize_by_category(geom_view_t *gv,
                                                      std::vector<size_t> &cat_data,
                                                      std::vector<vector3<float> > &clr) {
  if (!gv) return;

  for (size_t i = 0; i < gv->m_geom->nat(); i++)
    if (cat_data[i] >= 0 && cat_data[i] < clr.size())
      gv->set_xcolorv(i, clr[cat_data[i]]);

  gv->m_color_mode.set_value(geom_view_color_e::color_from_xgeom);

}

void geom_view_colorizer_helper::colorize_by_sublattices(geom_view_t *gv, float eps) {

  if (!gv) return;
  auto sl1 = geom_view_tools_t::get_atoms_sublattices(gv, eps);

  std::vector<vector3<float>> colors;
  colors.resize(10);
  colors[0]  = {  32.0 / 255.0,  32.0 / 255.0,  32.0 / 255.0 };
  colors[1]  = {  62.0 / 255.0,   6.0 / 255.0,   7.0 / 255.0 };
  colors[2]  = {  55.0 / 255.0, 126.0 / 255.0, 184.0 / 255.0 };
  colors[3]  = {  77.0 / 255.0, 175.0 / 255.0,  74.0 / 255.0 };
  colors[4]  = { 152.0 / 255.0,  78.0 / 255.0, 163.0 / 255.0 };
  colors[5]  = { 255.0 / 255.0, 127.0 / 255.0,   0.0 / 255.0 };
  colors[6]  = { 255.0 / 255.0, 255.0 / 255.0,  51.0 / 255.0 };
  colors[7]  = { 166.0 / 255.0,  86.0 / 255.0,  40.0 / 255.0 };
  colors[8]  = { 247.0 / 255.0, 129.0 / 255.0, 191.0 / 255.0 };
  colors[9]  = { 166.0 / 255.0, 206.0 / 255.0, 227.0 / 255.0 };

  geom_view_colorizer_helper::colorize_by_category(gv, sl1, colors);

}
