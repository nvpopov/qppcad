#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/sflow/sflow_base_nodes_int.hpp>
#include <qppcad/sflow/sflow_base_nodes_float.hpp>

#include <qppcad/ws_item/node_book/node_book_graphics_view.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_t::node_book_t() {

  set_default_flags(ws_item_flags_default);

  m_scene = new node_book_graphics_scene_t(nullptr);
  m_scene->m_parent_node_book = this;

  m_sflow_context = std::make_shared<sflow_context_t>();

}

void node_book_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                        vector3<float> &out_look_at) {
  //do nothing
}

void node_book_t::render() {
  //do nothing
}

bool node_book_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string node_book_t::compose_type_descr() {
  return "node_book";
}

void node_book_t::update(float delta_time) {

  //do nothing
  ws_item_t::update(delta_time);

  if (m_sflow_context->is_finished())
    update_output_values();

}

float node_book_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t node_book_t::get_num_cnt_selected() {
  return 0;
}

size_t node_book_t::get_content_count() {
  return 0;
}

void node_book_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);

}

void node_book_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);

}

void node_book_t::execute() {

  if (!m_sflow_context) return;

  m_sflow_context->clear_context();
  m_sflow_context->m_nodes.clear();

  for (auto &elem : m_scene->m_nodes)
    m_sflow_context->add_node(elem->m_sf_node);

  for (auto &con : m_scene->m_connections)
    if (con->m_inp_socket && con->m_out_socket)
      m_sflow_context->connect_node(con->m_out_socket->m_node->m_sf_node,
                                    con->m_inp_socket->m_node->m_sf_node,
                                    con->m_out_socket->m_socket_id,
                                    con->m_inp_socket->m_socket_id);

  m_sflow_context->execute_threaded();

  //update_output_values();

}

void node_book_t::post_execute() {

}

void node_book_t::update_output_values() {

  //update output values
  for (auto &elem : m_scene->m_nodes)
    for (size_t i = 0; i < elem->m_inplace_wdgts.size(); i++)
      if (elem->m_inplace_wdgts[i]) {

          switch (elem->m_sf_node->m_ipl_types[i].m_type) {

            case sflow_parameter_e::sfpar_int : {
                qbinded_int_spinbox_t *c_int_sb =
                    qobject_cast<qbinded_int_spinbox_t*>(elem->m_inplace_wdgts[i]);
                if (c_int_sb) c_int_sb->load_value_ex();
                break;
              }

            case sflow_parameter_e::sfpar_float : {
                qbinded_float_spinbox_t *c_f_sb =
                    qobject_cast<qbinded_float_spinbox_t*>(elem->m_inplace_wdgts[i]);
                if (c_f_sb) c_f_sb->load_value_ex();
                break;
              }

            case sflow_parameter_e::sfpar_v3f : {
                qbinded_float3_input_t *c_v3f =
                    qobject_cast<qbinded_float3_input_t*>(elem->m_inplace_wdgts[i]);
                if (c_v3f) c_v3f->load_value_ex();
                break;
              }

              //              case sflow_parameter_e::sfpar_ws_item : {
              //                  qbinded_ws_item_combobox_t *c_wsi =
              //                      qobject_cast<qbinded_ws_item_combobox_t*>(wdgt);
              //                  if (c_wsi) c_wsi->load_value();
              //                  break;
              //                }

            case sflow_parameter_e::sfpar_bool : {
                break;
              }

            default : {
                break;
              }

            }

        }

}

void node_book_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

  if (m_auto_recompute && (update_reason & ws_item_updf_regenerate_content)) execute();

}

py::list node_book_t::py_get_visible_rect() {

  app_state_t *astate = app_state_t::get_inst();

  auto views = m_scene->views();

  astate->tlog("@DEBUG: node_book_t::py_get_visible_rect -> views.size={}", views.size());
  if (views.empty()) return py::none();
  astate->tlog("@DEBUG: node_book_t::py_get_visible_rect -> views!empty");

  auto nb_view = qobject_cast<node_book_graphics_view_t*>(views.first());
  if (!nb_view) return py::none();
  astate->tlog("@DEBUG: node_book_t::py_get_visible_rect -> nb_view");

  auto vis_rect = nb_view->get_visible_rect();

  py::list ret;
  ret.append(vis_rect.left());
  ret.append(vis_rect.top());
  ret.append(vis_rect.right());
  ret.append(vis_rect.bottom());

  return ret;

}
