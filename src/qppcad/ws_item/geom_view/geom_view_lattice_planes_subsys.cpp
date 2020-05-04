#include <qppcad/ws_item/geom_view/geom_view_lattice_planes_subsys.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp::cad;


geom_view_lat_planes_subsys_t::geom_view_lat_planes_subsys_t(geom_view_t &_p_owner) {
  p_owner = &_p_owner;
}

void geom_view_lat_planes_subsys_t::add_plane(int _h, int _k, int _l) {
  if (_h == 0 || _k == 0 || _l == 0) return;
  m_planes.emplace_back(p_owner->m_geom.get(), _h, _k, _l);
}

void geom_view_lat_planes_subsys_t::render() {
  if (p_owner->m_geom->DIM == 0) return;
  app_state_t* astate = app_state_t::get_inst();

  astate->dp->begin_render_general_mesh();
  astate->glapi->glDisable(GL_CULL_FACE);
  float specular_alpha = 0.0f;
  astate->sp_mvp_ssl->set_u(sp_u_name::f_specular_alpha, &specular_alpha);

  vector3<float> scale{1.0f, 1.0f, 1.0f};

  for (auto &plane : m_planes) {
      scale = vector3<float>(plane.m_size, plane.m_size, plane.m_size);
      astate->dp->render_general_mesh(p_owner->m_pos.get_value() + plane.m_center,
                                      scale, plane.m_rotation,
                                      clr_green, astate->mesh_zup_quad);
    }

  astate->dp->end_render_general_mesh();
  astate->glapi->glEnable(GL_CULL_FACE);
}

//void geom_view_lat_planes_subsys_t::render_ui_obj_insp() {

//  app_state_t* astate = &(c_app::get_state());

//  if ( p_owner->m_geom->DIM > 0)
//    if (ImGui::CollapsingHeader("Lattice planes")) {

//        ImGui::Spacing();
//        ImGui::Separator();
//        static int _hkl[3];

//        ImGui::Spacing();
//        ImGui::BulletText("Add new lattice plane");
//        ImGui::PushItemWidth(90);
//        ImGui::InputInt3("<hkl>", _hkl);

//        ImGui::PopItemWidth();

//        ImGui::SameLine();
//        if (ImGui::Button("Add plane")) {
//            add_plane(_hkl[0], _hkl[1], _hkl[2]);
//            astate->make_viewport_dirty();
//          }
//        ImGui::Spacing();
//        ImGui::Separator();
//      }
//}

