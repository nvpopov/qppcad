#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ui/qextended_action.hpp>

using namespace qpp;
using namespace qpp::cad;


node_book_graphics_scene_t::node_book_graphics_scene_t(QObject *parent)
  : QGraphicsScene(parent) ,
    p_clr_bckgr(QColor("#393939")),
    p_clr_light(QColor("#2F2F2F")),
    p_clr_dark(QColor("#292929")),
    p_pen_light(QPen(p_clr_light)),
    p_pen_dark(QPen(p_clr_dark)),
    p_brush_bckgr(p_clr_bckgr) {

  app_state_t *astate = app_state_t::get_inst();

  for (auto p : {&p_pen_light, &p_pen_dark}) p->setWidth(0);
  setBackgroundBrush(p_brush_bckgr);

  gs_qnode_menu = new QMenu;
  gs_qnode_menu_delete = new QAction("Delete");
  gs_qnode_menu_unlink_all = new QAction("Unlink all");
  gs_qnode_menu->addAction(gs_qnode_menu_delete);
  gs_qnode_menu->addAction(gs_qnode_menu_unlink_all);

  gs_global_menu = new QMenu;

  std::map<size_t, QMenu*> _sflow_grp_lookup;

  for (auto &rec : astate->ws_mgr->m_bhv_mgr->m_sflow_node_group_info) {
      QMenu *_temp = gs_global_menu->addMenu(QString::fromStdString(rec.second.m_group_name));
      _sflow_grp_lookup[rec.first] = _temp;
    }

  for (auto &rec : astate->ws_mgr->m_bhv_mgr->m_sflow_node_info) {
      auto it = _sflow_grp_lookup.find(rec.second.m_group_hash);
      if (it != _sflow_grp_lookup.end()) {
          qextended_action *_temp_act = new qextended_action();
          _temp_act->setText(QString::fromStdString(rec.second.m_full_name));
          _temp_act->m_joined_data[0] = rec.first;
          if (it->second) it->second->addAction(_temp_act);
        }
    }

  setSceneRect(-5000, -5000, 2*5000, 2*5000);

}

void node_book_graphics_scene_t::add_connection(std::shared_ptr<qnode_connection_t> _con) {

  if (_con) {
      m_connections.push_back(_con);
      addItem(_con.get());
    }

}

void node_book_graphics_scene_t::add_node(std::shared_ptr<qnode_t> _node) {

  if (_node) {
      addItem(_node.get());
      _node->m_scene = this;
      m_nodes.push_back(_node);
      for (auto elem : _node->m_inp_sockets) m_sockets.push_back(elem);
      for (auto elem : _node->m_out_sockets) m_sockets.push_back(elem);
    }

}

void node_book_graphics_scene_t::update_connections_with_node(std::shared_ptr<qnode_t> _node) {

  for (auto elem : m_connections)
    if (elem && elem->m_inp_socket && elem->m_out_socket &&
        elem->m_inp_socket->m_node && elem->m_out_socket->m_node &&
        (elem->m_inp_socket->m_node == _node || elem->m_out_socket->m_node == _node)) {
        elem->update_path(QPointF(0,0), true);
      }
}

void node_book_graphics_scene_t::delete_node(qnode_t *_node) {

  if (!_node) return;

  unlink_node(_node);

  //first delete node with sockets from graphics scene
  for (auto it = m_nodes.begin(); it != m_nodes.end();)
    if ((*it) && (*it).get() == _node) {
        removeItem((*it).get());
        it = m_nodes.erase(it);
      } else {
        ++it;
      }

  //now raii shared ptrs
  for (auto it = m_sockets.begin(); it != m_sockets.end();)
    if ((*it) && (*it)->m_node.get() == _node) {
        it = m_sockets.erase(it);
      } else {
        ++it;
      }

}

void node_book_graphics_scene_t::unlink_node(qnode_t *_node) {

  if (!_node) return;

  for (auto it = m_connections.begin(); it != m_connections.end();)

    if (((*it) && (*it)->m_inp_socket && (*it)->m_inp_socket->m_node.get() == _node) ||
        ((*it) && (*it)->m_out_socket && (*it)->m_out_socket->m_node.get() == _node)) {

        removeItem((*it).get());

        if ((*it)->m_inp_socket->m_node.get() == _node)
          notify_linked_nodes_about_unlinking((*it)->m_out_socket->m_node.get());
        else
          notify_linked_nodes_about_unlinking((*it)->m_inp_socket->m_node.get());

        it = m_connections.erase(it);

      } else {

        ++it;

      }

}

void node_book_graphics_scene_t::notify_linked_nodes_about_unlinking(qnode_t *_node) {

  if (!_node) return;

  for (size_t i = 0; i < _node->m_inplace_wdgts.size(); i++)
    if (_node->m_inplace_wdgts[i] && !_node->m_sf_node->m_ipl_types[i].m_editable) {

          switch (_node->m_sf_node->m_ipl_types[i].m_type) {

            case sflow_parameter_e::sfpar_int : {
                qbinded_int_spinbox_t *c_int_sb =
                    qobject_cast<qbinded_int_spinbox_t*>(_node->m_inplace_wdgts[i]);
                if (c_int_sb) {
                    *(c_int_sb->m_binded_value) = 0;
                    c_int_sb->load_value_ex();
                  }
                break;
              }

            case sflow_parameter_e::sfpar_float : {
                qbinded_float_spinbox_t *c_f_sb =
                    qobject_cast<qbinded_float_spinbox_t*>(_node->m_inplace_wdgts[i]);
                if (c_f_sb) {
                    *(c_f_sb->m_binded_value) = 0;
                    c_f_sb->load_value_ex();
                  }
                break;
              }

            case sflow_parameter_e::sfpar_v3f : {
                qbinded_float3_input_t *c_v3f =
                    qobject_cast<qbinded_float3_input_t*>(_node->m_inplace_wdgts[i]);
                if (c_v3f) {
                    *(c_v3f->m_binded_value) = vector3<float>{0};
                    c_v3f->load_value_ex();
                  }
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

void node_book_graphics_scene_t::construct_new_node(QPointF pos, size_t sflow_fbr_hash) {

  app_state_t* astate = app_state_t::get_inst();

  auto it = astate->ws_mgr->m_bhv_mgr->m_sflow_node_info.find(sflow_fbr_hash);
  if (it == astate->ws_mgr->m_bhv_mgr->m_sflow_node_info.end()) return;

  auto new_node = it->second.m_fabric();
  auto new_qnode = std::make_shared<qnode_t>();

  // set m_scene for connecting qbinded_input_* ws_item_t
  new_qnode->m_scene = this;

  // form node geometry first
  new_qnode->set_sflow_node(new_node);

  // then submit node to qgraphics_scene
  add_node(new_qnode);
  new_qnode->setPos(pos);

}

void node_book_graphics_scene_t::drawBackground(QPainter *painter, const QRectF &rect) {

  QGraphicsScene::drawBackground(painter, rect);

  const int gridsize = 20;
  int left = static_cast<int>(std::floor(rect.left()));
  int right = static_cast<int>(std::ceil(rect.right()));
  int top = static_cast<int>(std::floor(rect.top()));
  int bottom = static_cast<int>(std::ceil(rect.bottom()));

  const int first_left = left - (left % gridsize);
  const int first_top = top - (top % gridsize);

  std::vector<QLine> lines_light;
  std::vector<QLine> lines_dark;

  for (int x = first_left; x <= right; x += gridsize) {
      if (x % 100 != 0) lines_light.push_back(QLine(x, top, x, bottom));
      else lines_dark.push_back(QLine(x, top, x, bottom));
    }

  for (int y = first_top; y <= bottom; y += gridsize) {
      if (y % 100 != 0) lines_light.push_back(QLine(left, y, right, y));
      else lines_dark.push_back(QLine(left, y, right, y));
    }

  painter->setPen(p_pen_light);
  painter->drawLines(lines_light.data(), lines_light.size());

  painter->setPen(p_pen_dark);
  painter->drawLines(lines_dark.data(), lines_dark.size());

}

bool node_book_graphics_scene_t::event(QEvent *event) {

  app_state_t *astate = app_state_t::get_inst();
  return QGraphicsScene::event(event);

}

void node_book_graphics_scene_t::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {

  QList<QGraphicsItem *> items_uc = items(event->scenePos());

  if (items_uc.empty()) {
      auto res = gs_global_menu->exec(event->screenPos());
      if (res) {
          qextended_action *ext_act = qobject_cast<qextended_action*>(res);
          if (ext_act) construct_new_node(event->scenePos(), ext_act->m_joined_data[0]);
        }
    } else {

       QGraphicsItem *item_qnode{nullptr};
       for (auto _item : items_uc) {
           if (_item && _item->type() == qnode_t::Type)
             item_qnode = _item;
         }

       if (item_qnode) {
           auto retv = gs_qnode_menu->exec(event->screenPos());
           qnode_t* item_qnode_casted = qgraphicsitem_cast<qnode_t*>(item_qnode);
           if (retv == gs_qnode_menu_delete) delete_node(item_qnode_casted);
           if (retv == gs_qnode_menu_unlink_all) unlink_node(item_qnode_casted);
         }

    } // else

}
