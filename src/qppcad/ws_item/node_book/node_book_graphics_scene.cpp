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

}

void node_book_graphics_scene_t::add_connection(qnode_connection_t *_con) {

  if (_con) {
      m_connections.push_back(_con);
      addItem(_con);
    }

}

void node_book_graphics_scene_t::add_node(std::shared_ptr<qnode_t> _node) {

  if (_node) {
      addItem(_node.get());
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

void node_book_graphics_scene_t::remove_node(qnode_t *_node) {

  if (!_node) return;

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
  astate->tlog("node_book_graphics_scene_t::event(QEvent) {}", event->type());
  return QGraphicsScene::event(event);

}
