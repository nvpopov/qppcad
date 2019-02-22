#include <qppcad/psg_view/psg_view.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

psg_view_t::psg_view_t() {
  set_default_flags(ws_item_flags_default);
}

void psg_view_t::gen_from_geom(xgeometry<float, periodic_cell<float> > &geom,
                               float tolerance,
                               bool nested) {

  //app_state_t* astate = app_state_t::get_inst();

  if (!m_ag) m_ag = std::make_shared<array_group<matrix3<float> > > ();
  find_point_symm(*m_ag, geom, m_new_centre, tolerance);
  m_pg_axes = point_group_axes<float>(*m_ag);
  m_atf.clear();

  for (int i = 0; i < m_pg_axes.axes.size(); i++) {
      transform_record_t new_tr;
      new_tr.m_is_plane = false;
      new_tr.m_axis = m_pg_axes.axes[i];
      new_tr.m_color = {1,0,0};
      std::cout << "PGAXES " << m_pg_axes.orders[i] << " " << m_pg_axes.axes[i] << std::endl;
      m_atf.push_back(std::move(new_tr));
    }

  for (int i = 0; i < m_pg_axes.planes.size(); i++) {
      transform_record_t new_tr;
      new_tr.m_is_plane = true;
      new_tr.m_axis =  m_pg_axes.planes[i];
      new_tr.m_color = {0, 1, 0};
      std::cout << "PGPLANES " << m_pg_axes.planes[i] << std::endl;
      m_atf.push_back(std::move(new_tr));
    }

  recalc_render_data();

}

void psg_view_t::recalc_render_data() {

  for (auto &elem : m_atf) {

      vector3<float> aliasing{(static_cast <float> (rand()) /
            static_cast <float> (RAND_MAX))*0.05f};

      vector3<float> start = (elem.m_axis * 5);
      vector3<float> end = (elem.m_axis * -5);

      vector3<float> tr = m_new_centre;

      //threat first connected item as master
      if (!m_connected_items.empty()) tr += m_connected_items.front()->m_pos;

      start += tr;
      end += tr;

      float scale_val = m_axis_scale * 0.02f;

      if (elem.m_is_plane) {

          start += aliasing;
          matrix4<float> mat_model = matrix4<float>::Identity();
          vector3<float> plane_normal_ut = vector3<float>(0, 0, 1);
          vector3<float> plane_rot_axis = plane_normal_ut.cross(elem.m_axis).normalized();
          float len_mod = elem.m_axis.norm() * plane_normal_ut.norm();
          float rot_angle = std::acos(plane_normal_ut.dot(elem.m_axis) / len_mod);
          Eigen::Affine3f t;
          t = Eigen::AngleAxisf(rot_angle, plane_rot_axis);
          t.prescale(vector3<float>(m_plane_scale));
          t.pretranslate(tr);
          mat_model = t.matrix();
          elem.m_render_mat = mat_model;

        } else {

          matrix4<float> mat_model = matrix4<float>::Identity();
          start *= m_axis_len_mod;
          end *= m_axis_len_mod;

          mat_model.block<3,1>(0,3) = start;
          mat_model.block<3,1>(0,2) = end - start;

          vector3<float> vec_axis_norm = mat_model.block<3,1>(0,2).normalized();

          mat_model.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * scale_val;
          mat_model.block<3,1>(0,1) = vec_axis_norm.cross(mat_model.block<3,1>(0,0));
          mat_model.block<3,1>(0,3) = start ;

          elem.m_render_mat = mat_model;

          //aux
          vector3<float> start_aux = elem.m_axis * 5 + tr;
          vector3<float> end_aux = elem.m_axis * 5.25 + tr;

          start_aux *= m_axis_len_mod;
          end_aux *= m_axis_len_mod;

          matrix4<float> mat_model_aux = matrix4<float>::Identity();
          mat_model_aux.block<3,1>(0,3) = start_aux;
          mat_model_aux.block<3,1>(0,2) = end_aux - start_aux;

          vector3<float> vec_axis_norm_aux = mat_model_aux.block<3,1>(0,2).normalized();
          mat_model_aux.block<3,1>(0,0) = vec_axis_norm_aux.unitOrthogonal() * scale_val * 1.4;
          mat_model_aux.block<3,1>(0,1) = vec_axis_norm_aux.cross(mat_model_aux.block<3,1>(0,0));
          mat_model_aux.block<3,1>(0,3) = start_aux;

          elem.m_render_mat_aux = mat_model_aux;
        }

    }

}

void psg_view_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                       vector3<float> &out_look_at) {
  //do nothing
}

void psg_view_t::render() {

  if (!m_is_visible) return;

  app_state_t* astate = app_state_t::get_inst();

  //draw axes
  astate->dp->begin_render_general_mesh();

  for (auto &elem : m_atf)
    if (!elem.m_is_plane && elem.m_is_visible) {
        astate->dp->render_general_mesh(elem.m_render_mat,
                                        elem.m_color,
                                        astate->mesh_cylinder);
        astate->dp->render_general_mesh(elem.m_render_mat_aux,
                                        elem.m_color,
                                        astate->mesh_unit_cone);
      }

  astate->dp->end_render_general_mesh();

  if (m_plane_alpha_enabled) {

      astate->dp->begin_render_general_mesh(astate->sp_mvap_ssl);
      astate->dp->cull_func(draw_pipeline_cull_func::cull_disable);
      for (auto &elem : m_atf)
        if (elem.m_is_plane && elem.m_is_visible) {
            astate->dp->render_general_mesh(elem.m_render_mat,
                                            elem.m_color,
                                            astate->mesh_zl_plane,
                                            m_plane_alpha,
                                            astate->sp_mvap_ssl);
          }
      astate->dp->cull_func(draw_pipeline_cull_func::cull_enable);
      astate->dp->end_render_general_mesh(astate->sp_mvap_ssl);

      //            vector3<float> plane_color = (plane_c % 2 == 0) ? clr_green : clr_blue;
      //            astate->dp->render_general_mesh(elem.m_render_mat, plane_color, astate->mesh_zl_plane,
      //                                            m_plane_alpha, astate->sp_mvap_ssl);
      //          plane_c += 1;
      //          }
      //      astate->dp->end_no_cull();
      //      astate->dp->end_render_general_mesh(astate->sp_mvap_ssl);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_enabled);

      //1 pass depth buffer
      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_disable);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_less);
      //      for (auto &elem : m_atf)
      //        if (elem.m_is_plane && elem.m_is_visible) {
      //            astate->dp->render_general_mesh(elem.m_render_mat,
      //                                            elem.m_color,
      //                                            astate->mesh_zl_plane,
      //                                            0,
      //                                            astate->sp_mvap_ssl);
      //          }

      //      //2 pass
      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_enable);
      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_front);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_always);
      //      for (auto &elem : m_atf)
      //        if (elem.m_is_plane && elem.m_is_visible) {
      //            astate->dp->render_general_mesh(elem.m_render_mat,
      //                                            elem.m_color,
      //                                            astate->mesh_zl_plane,
      //                                            m_plane_alpha,
      //                                            astate->sp_mvap_ssl);
      //          }

      //      //3 pass
      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_front);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_lequal);
      //      for (auto &elem : m_atf)
      //        if (elem.m_is_plane && elem.m_is_visible) {
      //            astate->dp->render_general_mesh(elem.m_render_mat,
      //                                            elem.m_color,
      //                                            astate->mesh_zl_plane,
      //                                            m_plane_alpha,
      //                                            astate->sp_mvap_ssl);
      //          }

      //      //4 pass cull back depth always
      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_back);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_always);
      //      for (auto &elem : m_atf)
      //        if (elem.m_is_plane && elem.m_is_visible) {
      //            astate->dp->render_general_mesh(elem.m_render_mat,
      //                                            elem.m_color,
      //                                            astate->mesh_zl_plane,
      //                                            m_plane_alpha,
      //                                            astate->sp_mvap_ssl);
      //          }

      //      //5 pass cull back depth always
      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_disable);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_lequal);
      //      for (auto &elem : m_atf)
      //        if (elem.m_is_plane && elem.m_is_visible) {
      //            astate->dp->render_general_mesh(elem.m_render_mat,
      //                                            elem.m_color,
      //                                            astate->mesh_zl_plane,
      //                                            m_plane_alpha,
      //                                            astate->sp_mvap_ssl);
      //          }
      //      //restore default culling

      //      astate->dp->cull_func(draw_pipeline_cull_func::cull_enable);
      //      astate->dp->depth_func(draw_pipeline_depth_func::depth_lequal);

      //      astate->dp->end_render_general_mesh(astate->sp_mvap_ssl);

    } else {
      astate->dp->begin_render_general_mesh();
      astate->dp->cull_func(draw_pipeline_cull_func::cull_disable);
      for (auto &elem : m_atf)
        if (elem.m_is_plane && elem.m_is_visible) {
            astate->dp->render_general_mesh(elem.m_render_mat,
                                            elem.m_color,
                                            astate->mesh_zl_plane);
          }
      astate->dp->cull_func(draw_pipeline_cull_func::cull_enable);
      astate->dp->end_render_general_mesh();
    }


}

bool psg_view_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string psg_view_t::compose_item_name() {
  return "point sym. group view";
}

void psg_view_t::update(float delta_time) {
  //do nothing
}

float psg_view_t::get_bb_prescaller() {
  return 1.0f;
}

void psg_view_t::updated_internally() {
  recalc_render_data();
}

uint32_t psg_view_t::get_amount_of_selected_content() {
  return 0;
}

size_t psg_view_t::get_content_count() {
  return 0;
}

void psg_view_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void psg_view_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void psg_view_t::py_update() {

  app_state_t* astate = app_state_t::get_inst();

  m_pg_axes = point_group_axes<float>(*m_ag);
  m_atf.clear();

  for (int i = 0; i < m_pg_axes.axes.size(); i++) {
      transform_record_t new_tr;
      new_tr.m_is_plane = false;
      new_tr.m_axis = m_pg_axes.axes[i];
      new_tr.m_color = {1,0,0};
      std::cout << "PGAXES " << m_pg_axes.orders[i] << " " << m_pg_axes.axes[i] << std::endl;
      m_atf.push_back(std::move(new_tr));
    }

  for (int i = 0; i < m_pg_axes.planes.size(); i++) {
      transform_record_t new_tr;
      new_tr.m_is_plane = true;
      new_tr.m_axis =  m_pg_axes.planes[i];
      new_tr.m_color = {0, 1, 0};
      std::cout << "PGPLANES " << m_pg_axes.planes[i] << std::endl;
      m_atf.push_back(std::move(new_tr));
    }

  recalc_render_data();

  astate->astate_evd->cur_ws_selected_item_changed();
  astate->make_viewport_dirty();

}



