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

  if (m_sflow_context->is_finished()) {
      update_output_values();
      if (m_sflow_context->is_force_execute()) execute();
    }

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
  json nodes = json::array({});

  auto bhv_mgr = app_state_t::get_inst()->ws_mgr->m_bhv_mgr;

  //nodes lookup
  std::map<qnode_t*, size_t> nds_lkp;

  //socket lookup
  std::map<qnode_socket_t*, size_t> isck_lkp, osck_lkp;

  //store nodes
  for (size_t i = 0; i < m_scene->m_nodes.size(); i++) {

      nds_lkp.insert({m_scene->m_nodes[i].get(), i});

      for (size_t q = 0; q < m_scene->m_nodes[i]->m_inp_sockets.size(); q++)
        isck_lkp.insert({m_scene->m_nodes[i]->m_inp_sockets[q].get(), q});

      for (size_t q = 0; q < m_scene->m_nodes[i]->m_out_sockets.size(); q++)
        osck_lkp.insert({m_scene->m_nodes[i]->m_out_sockets[q].get(), q});

      auto node = m_scene->m_nodes[i]->m_sf_node;

      json node_json;
      node_json[JSON_NODE_BOOK_NODE_TYPEHASH] = node->m_node_type_hash;

      //get node type name
      auto it1 = bhv_mgr->m_sflow_node_info.find(node->m_node_type_hash);

      if (it1 != bhv_mgr->m_sflow_node_info.end()) {
          node_json[JSON_NODE_BOOK_NODE_TYPENAME] = it1->second.m_full_name;
        }

      json coords = json::array({});
      coords.push_back(m_scene->m_nodes[i]->x());
      coords.push_back(m_scene->m_nodes[i]->y());

      node_json[JSON_NODE_BOOK_NODE_COORD] = coords;

      nodes.push_back(node_json);

    }

  data[JSON_NODE_BOOK_NODES] = nodes;

  //store connections
  json connections = json::array({});

  for (size_t i = 0; i < m_scene->m_connections.size(); i++) {

      auto connection = m_scene->m_connections[i];
      auto inode_itr = nds_lkp.find(connection->m_inp_socket->m_node.get());
      auto onode_itr = nds_lkp.find(connection->m_out_socket->m_node.get());
      auto isck_itr  = isck_lkp.find(connection->m_inp_socket);
      auto osck_itr  = osck_lkp.find(connection->m_out_socket);

      if (inode_itr != nds_lkp.end() && onode_itr != nds_lkp.end()
          && isck_itr != isck_lkp.end() && osck_itr != osck_lkp.end()) {

          json connection_json;
          connection_json[JSON_NODE_BOOK_CONNECTIONS_INODE] = inode_itr->second;
          connection_json[JSON_NODE_BOOK_CONNECTIONS_ISCK] = isck_itr->second;
          connection_json[JSON_NODE_BOOK_CONNECTIONS_ONODE] = onode_itr->second;
          connection_json[JSON_NODE_BOOK_CONNECTIONS_OSCK] = osck_itr->second;

          connections.push_back(connection_json);

        }

    }

  data[JSON_NODE_BOOK_CONNECTIONS] = connections;

}

void node_book_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  auto bhv_mgr = app_state_t::get_inst()->ws_mgr->m_bhv_mgr;

  ws_item_t::load_from_json(data, rep_info);

  std::map<qnode_t*, size_t> nds_lkp;
  size_t nds_idx{0};

  //std::vector<std::array<size_t, 4> > con_restore;

  auto nodes_itr = data.find(JSON_NODE_BOOK_NODES);
  if (nodes_itr != data.end())
    for (auto &node_ref : nodes_itr.value()) {

        auto nodehash_itr = node_ref.find(JSON_NODE_BOOK_NODE_TYPEHASH);
        auto pos_itr = node_ref.find(JSON_NODE_BOOK_NODE_COORD);
        if (pos_itr != node_ref.end() && nodehash_itr != node_ref.end()) {

            qreal node_x = pos_itr.value()[0].get<double>();
            qreal node_y = pos_itr.value()[1].get<double>();

            auto new_node = m_scene->construct_new_node(QPointF(node_x, node_y),
                                                        nodehash_itr.value().get<size_t>());

            nds_lkp.insert({new_node.get(), nds_idx});
            nds_idx++;

          }

      }

  auto connection_itr = data.find(JSON_NODE_BOOK_CONNECTIONS);
  if (connection_itr != data.end()) {

      for (auto &connection : connection_itr.value()) {

          auto inode_itr = connection.find(JSON_NODE_BOOK_CONNECTIONS_INODE);
          auto onode_itr = connection.find(JSON_NODE_BOOK_CONNECTIONS_ONODE);
          auto isck_itr = connection.find(JSON_NODE_BOOK_CONNECTIONS_ISCK);
          auto osck_itr = connection.find(JSON_NODE_BOOK_CONNECTIONS_OSCK);

          if (inode_itr != connection.end() && onode_itr != connection.end()
              && isck_itr != connection.end() && osck_itr != connection.end()) {
              m_scene->add_connection(inode_itr.value().get<size_t>(),
                                      onode_itr.value().get<size_t>(),
                                      isck_itr.value().get<size_t>(),
                                      osck_itr.value().get<size_t>());
//                            fmt::print("@@@@@ {} {} {} {}",inode_itr.value().get<size_t>(),
//                                                    onode_itr.value().get<size_t>(),
//                                                    isck_itr.value().get<size_t>(),
//                                                    osck_itr.value().get<size_t>());
            }


        }

    }

}

bool node_book_t::can_be_written_to_json() {
  return true;
}

void node_book_t::execute() {

  if (!m_sflow_context) return;

  if (m_sflow_context->is_running()) {
      m_sflow_context->force_execute();
      return;
    }

  m_sflow_context->clear_context();

  for (auto &elem : m_scene->m_nodes)
    m_sflow_context->add_node(elem->m_sf_node);

  for (auto &con : m_scene->m_connections)
    if (con->m_inp_socket && con->m_out_socket)
      m_sflow_context->connect_node(con->m_out_socket->m_node->m_sf_node,
                                    con->m_inp_socket->m_node->m_sf_node,
                                    con->m_out_socket->m_socket_id,
                                    con->m_inp_socket->m_socket_id);

  m_sflow_context->execute_threaded();

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
