#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/sflow/sflow_base_nodes_int.hpp>
#include <qppcad/sflow/sflow_base_nodes_float.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_t::node_book_t() {

  set_default_flags(ws_item_flags_default);

  m_scene = new node_book_graphics_scene_t(nullptr);
  m_scene->m_parent_node_book = this;

  m_sflow_context = std::make_shared<sflow_context_t>();

  auto new_node1 = std::make_shared<sf_int_prop_node_t>();
  auto qnode1 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode1);
  qnode1->set_sflow_node(new_node1);
  qnode1->setPos(QPoint(0,0));

  auto new_node2 = std::make_shared<sf_int_final_node_t>();
  auto qnode2 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode2);
  qnode2->set_sflow_node(new_node2);
  qnode2->setPos(QPoint(300,0));

  auto new_node3 = std::make_shared<sf_int_p_const_node_t>();
  auto qnode3 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode3);
  qnode3->setPos(QPoint(600,0));
  qnode3->set_sflow_node(new_node3);

  auto new_node4 = std::make_shared<sf_int_sum_int_node_t>();
  auto qnode4 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode4);
  qnode4->setPos(QPoint(0,300));
  qnode4->set_sflow_node(new_node4);

  auto new_node5 = std::make_shared<sf_float_prop_node_t>();
  auto qnode5 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode5);
  qnode5->setPos(QPoint(300,300));
  qnode5->set_sflow_node(new_node5);

  auto new_node6 = std::make_shared<sf_float_p_const_node_t>();
  auto qnode6 = std::make_shared<qnode_t>();
  m_scene->add_node(qnode6);
  qnode6->setPos(QPoint(600,300));
  qnode6->set_sflow_node(new_node6);

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

  m_sflow_context->execute();


  //update output values
  for (auto &elem : m_scene->m_nodes)
    for (auto wdgt : elem->m_inplace_wdgts)
      if (wdgt) {
          for (size_t i = 0; i < elem->m_sf_node->m_ipl.size(); i++)

            switch (elem->m_sf_node->m_ipl_types[i].m_type) {

              case sflow_parameter_e::sfpar_int : {
                  qbinded_int_spinbox_t *c_int_sb = qobject_cast<qbinded_int_spinbox_t*>(wdgt);
                  if (c_int_sb) c_int_sb->load_value_ex();
                  break;
                }

              case sflow_parameter_e::sfpar_float : {
                  qbinded_float_spinbox_t *c_f_sb = qobject_cast<qbinded_float_spinbox_t*>(wdgt);
                  if (c_f_sb) c_f_sb->load_value_ex();
                  break;
                }

              case sflow_parameter_e::sfpar_v3f : {
                  qbinded_float3_input_t *c_v3f = qobject_cast<qbinded_float3_input_t*>(wdgt);
                  if (c_v3f) c_v3f->load_value_ex();
                  break;
                }

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

  if (update_reason & ws_item_updf_regenerate_content) execute();

}

