#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/app_state.hpp>

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

  for (auto p : {&p_pen_light, &p_pen_dark}) p->setWidth(0);
  setBackgroundBrush(p_brush_bckgr);

  gs_global_menu = new QMenu;
  gs_global_menu->addAction("A1");
  gs_global_menu->addAction("A2");

  gs_qnode_menu = new QMenu;
  gs_qnode_menu_delete = new QAction("Delete");
  gs_qnode_menu_unlink_all = new QAction("Unlink all");
  gs_qnode_menu->addAction(gs_qnode_menu_delete);
  gs_qnode_menu->addAction(gs_qnode_menu_unlink_all);

}

void node_book_graphics_scene_t::add_connection(qnode_connection_t *_con) {

  if (_con) {
      m_connections.push_back(_con);
      addItem(_con);
    }

}

void node_book_graphics_scene_t::add_node(qnode_t *_node) {

  if (_node) {
      addItem(_node);
      _node->m_scene = this;
      m_nodes.push_back(_node);
    }

}

void node_book_graphics_scene_t::update_connections_with_node(qnode_t *_node) {

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

  for (auto it = m_sockets.begin(); it != m_sockets.end();)
    if ((*it) && (*it)->m_node == _node) {
        qnode_socket_t *_temp = (*it);
        it = m_sockets.erase(it);
        //removeItem(_temp);
      } else {
        ++it;
      }

  for (auto it = m_nodes.begin(); it != m_nodes.end();)
    if ((*it) && (*it) == _node) {
        qnode_t *_temp = (*it);
        it = m_nodes.erase(it);
        removeItem(_temp);
        delete _temp;
      } else {
        ++it;
      }

}

void node_book_graphics_scene_t::unlink_node(qnode_t *_node) {

  if (!_node) return;
  for (auto it = m_connections.begin(); it != m_connections.end();)
    if (((*it) && (*it)->m_inp_socket && (*it)->m_inp_socket->m_node == _node) ||
        ((*it) && (*it)->m_out_socket && (*it)->m_out_socket->m_node == _node)) {
        qnode_connection_t *_temp = (*it);
        it = m_connections.erase(it);
        removeItem(_temp);
        delete _temp;
      } else {
        ++it;
      }

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
  //astate->tlog("node_book_graphics_scene_t::event(QEvent) {}", event->type());
  return QGraphicsScene::event(event);

}

void node_book_graphics_scene_t::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {

  QList<QGraphicsItem *> items_uc = items(event->scenePos());
  if (items_uc.empty()) {
      gs_global_menu->exec(event->screenPos());
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

    }
}
