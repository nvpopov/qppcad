#include <qppcad/ws_point_sym_group/ws_point_sym_group.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_point_sym_group_t::ws_point_sym_group_t() {
  set_default_flags(ws_item_flags_default);
}

void ws_point_sym_group_t::gen_from_geom(xgeometry<float, periodic_cell<float> > &geom,
                                         float tolerance) {

  //app_state_t* astate = app_state_t::get_inst();

  find_point_symm(m_ag, geom, tolerance);
  m_pg_axes = point_group_axes<float>(m_ag);

  for (int i = 0; i < m_pg_axes.axes.size(); i++) {
      transform_record_t new_tr;
      new_tr.m_is_plane = false;
      new_tr.m_axis = m_pg_axes.axes[i];
      std::cout << "PGAXES " << m_pg_axes.orders[i] << " " << m_pg_axes.axes[i] << std::endl;
      m_atf.push_back(std::move(new_tr));
    }

  for (int i = 0; i < m_pg_axes.planes.size(); i++) {
      transform_record_t new_tr;
      new_tr.m_is_plane = true;
      new_tr.m_axis =  m_pg_axes.planes[i];
      std::cout << "PGPLANES " << m_pg_axes.planes[i] << std::endl;
      m_atf.push_back(std::move(new_tr));
    }

  recalc_render_data();

}

void ws_point_sym_group_t::recalc_render_data() {

  for (auto &elem : m_atf) {

      vector3<float> aliasing{(static_cast <float> (rand()) /
            static_cast <float> (RAND_MAX))*0.05f};

      vector3<float> start = elem.m_axis * 5 ;
      vector3<float> end = elem.m_axis * -5;
      float scale_val{0.055f};

      if (elem.m_is_plane) {

          start += aliasing;
          matrix4<float> mat_model = matrix4<float>::Identity();
          vector3<float> plane_normal_ut = vector3<float>(1, 0, 0);
          vector3<float> plane_rot_axis = plane_normal_ut.cross(elem.m_axis);
          float len_mod = elem.m_axis.norm() * plane_normal_ut.norm();
          float rot_angle = std::acos(plane_normal_ut.dot(elem.m_axis) / len_mod);
          Eigen::Affine3f t;
          t = Eigen::AngleAxisf(rot_angle, plane_rot_axis);
          t.prescale(vector3<float>(m_plane_scale));
         // t.translate(vector3<float>(0.5, 0.5, 0.5));
          mat_model = t.matrix();
          elem.m_render_mat = mat_model;

        } else {

          matrix4<float> mat_model = matrix4<float>::Identity();
          mat_model.block<3,1>(0,3) = start;
          mat_model.block<3,1>(0,2) = end - start;

          vector3<float> vec_axis_norm = mat_model.block<3,1>(0,2).normalized();

          mat_model.block<3,1>(0,0) = vec_axis_norm.unitOrthogonal() * scale_val;
          mat_model.block<3,1>(0,1) = vec_axis_norm.cross(mat_model.block<3,1>(0,0));
          mat_model.block<3,1>(0,3) = start ;

          elem.m_render_mat = mat_model;

          //aux

          vector3<float> start_aux = elem.m_axis * 5;
          vector3<float> end_aux = elem.m_axis * 5.25;

          matrix4<float> mat_model_aux = matrix4<float>::Identity();
          mat_model_aux.block<3,1>(0,3) = start_aux;
          mat_model_aux.block<3,1>(0,2) = end_aux - start_aux;

          vector3<float> vec_axis_norm_aux = mat_model_aux.block<3,1>(0,2).normalized();
          mat_model_aux.block<3,1>(0,0) = vec_axis_norm_aux.unitOrthogonal() * 0.1f;
          mat_model_aux.block<3,1>(0,1) = vec_axis_norm_aux.cross(mat_model_aux.block<3,1>(0,0));
          mat_model_aux.block<3,1>(0,3) = start_aux;

          elem.m_render_mat_aux = mat_model_aux;
        }

    }

}

void ws_point_sym_group_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                                 vector3<float> &out_look_at) {
  //do nothing
}

void ws_point_sym_group_t::render() {

  if (!m_is_visible) return;

  app_state_t* astate = app_state_t::get_inst();

  //draw axes
  astate->dp->begin_render_general_mesh();

  for (auto &elem : m_atf)
    if (!elem.m_is_plane) {
        if (elem.m_axis.isApprox(vector3<float>(0,0,1)) && !elem.m_inversion) {
            astate->dp->render_general_mesh(elem.m_render_mat, clr_olive,
                                            astate->mesh_cylinder);
            astate->dp->render_general_mesh(elem.m_render_mat_aux, clr_olive,
                                            astate->mesh_unit_cone);
          } else {
            astate->dp->render_general_mesh(elem.m_render_mat, clr_red,
                                            astate->mesh_cylinder);
            astate->dp->render_general_mesh(elem.m_render_mat_aux, clr_red,
                                            astate->mesh_unit_cone);
          }
      }

  astate->dp->end_render_general_mesh();

  astate->dp->begin_render_general_mesh(astate->sp_mvap_ssl);
  astate->dp->begin_no_cull();
  for (auto &elem : m_atf)
    if (elem.m_is_plane) {
        astate->dp->render_general_mesh(elem.m_render_mat, clr_green, astate->mesh_zl_plane,
                                        m_plane_alpha, astate->sp_mvap_ssl);
      }
  astate->dp->end_no_cull();
  astate->dp->end_render_general_mesh(astate->sp_mvap_ssl);

}

bool ws_point_sym_group_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string ws_point_sym_group_t::compose_item_name() {
  return "ws_point_sym_group";
}

void ws_point_sym_group_t::update(float delta_time) {
  //do nothing
}

float ws_point_sym_group_t::get_bb_prescaller() {
  return 1.0f;
}

void ws_point_sym_group_t::updated_internally() {

}

uint32_t ws_point_sym_group_t::get_amount_of_selected_content() {
  return 0;
}

size_t ws_point_sym_group_t::get_content_count() {
  return 0;
}

void ws_point_sym_group_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void ws_point_sym_group_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

