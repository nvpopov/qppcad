#include <qppcad/ws_atoms_list_lattice_planes_subsys.hpp>
#include <qppcad/ws_atoms_list.hpp>

using namespace qpp::cad;

template<typename REAL>
void lattice_plane_record_t<REAL>::update(geometry<REAL, periodic_cell<REAL> > *geom,
                                          int _h, int _k, int _l) {

  m_h = _h;
  m_k = _k;
  m_l = _l;

  vector3<REAL> pp1 = geom->cell.v[0] * m_h;
  vector3<REAL> pp2 = geom->cell.v[1] * m_k;
  vector3<REAL> pp3 = geom->cell.v[2] * m_l;

  m_size = 1.25f * std::max(geom->cell.v[0].norm(),
      std::max(geom->cell.v[1].norm(), geom->cell.v[2].norm()));

  m_center = (pp1 + pp2 + pp3) / 3.0f;

  float a1 = pp2[0] - pp1[0];
  float b1 = pp2[1] - pp1[1];
  float c1 = pp2[2] - pp1[2];
  float a2 = pp3[0] - pp1[0];
  float b2 = pp3[1] - pp1[1];
  float c2 = pp3[2] - pp1[2];
  a = b1 * c2 - b2 * c1;
  b = a2 * c1 - a1 * c2;
  c = a1 * b2 - b1 * a2;
  d = (- a * pp1[0] - b * pp1[1] - c * pp1[2]);

  m_normal = vector3<REAL>(a, b, c).normalized();

}

template<typename REAL>
lattice_plane_record_t<REAL>::lattice_plane_record_t(
    geometry<REAL, periodic_cell<REAL> > *geom,
    int _h, int _k, int _l) {

  update(geom, _h, _k, _l);

}

template<typename DATA, typename REAL, typename AINT>
void ws_atoms_list_lat_planes_subsys_t<DATA, REAL, AINT>::add_plane(int _h, int _k, int _l) {
  m_planes.emplace_back(p_owner->m_geom.get(), _h, _k, _l);
}

template<typename DATA, typename REAL, typename AINT>
void ws_atoms_list_lat_planes_subsys_t<DATA, REAL, AINT>::render() {
  if (p_owner->m_geom->DIM == 0) return;
  app_state_t* astate = &(c_app::get_state());

  astate->dp->begin_render_general_mesh();
  glDisable(GL_CULL_FACE);
  float specular_alpha = 0.0f;
  astate->mvp_ssl_program->set_u(sp_u_name::f_specular_alpha, &specular_alpha);

  vector3<float> scale{1.0f, 1.0f, 1.0f};

  for (auto &plane : m_planes) {
      scale = vector3<float>(plane.m_size, plane.m_size, plane.m_size);
      astate->dp->render_general_mesh(p_owner->m_pos + plane.m_center,
                                      scale, plane.m_normal,
                                      clr_green, astate->zup_quad);
    }
  astate->dp->end_render_general_mesh();
  glEnable(GL_CULL_FACE);
}

template<typename DATA, typename REAL, typename AINT>
void ws_atoms_list_lat_planes_subsys_t<DATA, REAL, AINT>::render_ui_obj_insp() {

  app_state_t* astate = &(c_app::get_state());

  if ( p_owner->m_geom->DIM > 0)
    if (ImGui::CollapsingHeader("Lattice planes")) {

        ImGui::Spacing();
        ImGui::Separator();
        static int _hkl[3];

        ImGui::BulletText("Add new lattice plane");
        ImGui::PushItemWidth(90);
        ImGui::InputInt3("<hkl>", _hkl);

        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("Add plane")) {
            add_plane(_hkl[0], _hkl[1], _hkl[2]);
            astate->make_viewport_dirty();
          }

        ImGui::Separator();
      }
}

template class qpp::cad::ws_atoms_list_lat_planes_subsys_t<ws_atoms_list_t, float>;
template struct qpp::cad::lattice_plane_record_t<float>;
