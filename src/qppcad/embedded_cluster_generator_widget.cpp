#include <qppcad/embedded_cluster_generator_widget.hpp>

using namespace qpp;
using namespace qpp::cad;

embedded_cluster_generator_widget_t::embedded_cluster_generator_widget_t (app_state_t *astate) {

}

void embedded_cluster_generator_widget_t::set_active (bool active) {
  m_active = active;
}

bool embedded_cluster_generator_widget_t::get_active () {
  return m_active;
}

void embedded_cluster_generator_widget_t::render () {

  if (!m_active) return;
  ImGui::Begin("Embedded cluster generator", &m_active);
  ImGui::Text("sds");
  ImGui::End();

}
