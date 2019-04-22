#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/app_state.hpp>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>

using namespace qpp;
using namespace qpp::cad;

qnode_connection_t::qnode_connection_t(QGraphicsItem *parent) : QGraphicsPathItem(parent) {

  setPen(QPen(m_connection_color, 2));
  setZValue(400);

}

qnode_connection_t::~qnode_connection_t() {

}

int qnode_connection_t::type() const {
  return Type;
}

void qnode_connection_t::update_path(QPointF point, bool finalize) {

  app_state_t *astate = app_state_t::get_inst();

  if (finalize && m_out_socket && m_inp_socket) {

      astate->tlog("qnode_connection_t::update_path() -> finalize");

      QPointF out_pos = (m_out_socket->scenePos());
      QPointF inp_pos = (m_inp_socket->scenePos());

      astate->tlog("OUTSCK ? = {}, INPSCK ? = {}, O == I ? {}",
                   m_out_socket != nullptr, m_inp_socket != nullptr, m_out_socket == m_inp_socket);
      astate->tlog("OUT_NN = {}, INP_NN = {}",
                   m_out_socket->m_node != nullptr,
                   m_inp_socket->m_node != nullptr);

      astate->tlog("qnode_connection_t::update_path() -> {} {} {} {}",
                   out_pos.x(), out_pos.y(), inp_pos.x(), inp_pos.y());

      out_pos += QPointF(6,6);
      inp_pos += QPointF(6,6);

      QPainterPath p;
      setPos(out_pos);

      QPointF inp_pos_pr = mapFromScene(inp_pos);

      qreal dx = inp_pos_pr.x();
      qreal dy = inp_pos_pr.y();

      QPointF ctr1(dx * 0.25, dy * 0.1);
      QPointF ctr2(dx * 0.75, dy * 0.9);

      p.cubicTo(ctr1, ctr2, inp_pos_pr);

      setPath(p);
      return;

    }

  if (!finalize && ((m_out_socket && !m_inp_socket) || (!m_out_socket && m_inp_socket))) {

      QPointF node_pos_um;

      if (m_out_socket) node_pos_um = (m_out_socket->scenePos());
      if (m_inp_socket) node_pos_um = (m_inp_socket->scenePos());

      node_pos_um += QPointF(6,6);

      QPainterPath p;
      setPos(node_pos_um);

      QPointF cpos = mapFromScene(point);

      astate->tlog("CPOS.X = {}, CPOS.Y = {}", cpos.x(), cpos.y());

      qreal dx = cpos.x();
      qreal dy = cpos.y();

      QPointF ctr1( dx * 0.25,  dy * 0.1);
      QPointF ctr2( dx * 0.75,  dy * 0.9);

      p.cubicTo(ctr1, ctr2, cpos);

      setPath(p);
      return;
    }

}

void qnode_connection_t::clear_path() {

  QPainterPath p;
  setPath(p);

}

bool qnode_connection_t::is_connection_valid() {

  if (!m_inp_socket) return false;
  if (!m_out_socket) return false;
  if (!m_inp_socket->m_node) return false;
  if (!m_out_socket->m_node) return false;
  if (m_inp_socket->m_node == m_out_socket->m_node) return false;

  if (m_inp_socket->m_is_inp_socket == m_out_socket->m_is_inp_socket) return false;
  if (!m_inp_socket->m_node->m_sf_node) return false;
  if (!m_out_socket->m_node->m_sf_node) return false;

  auto _inp_sck_id = m_inp_socket->m_socket_id;
  auto _out_sck_id = m_out_socket->m_socket_id;

  if (_inp_sck_id >= m_inp_socket->m_node->m_sf_node->m_inp_types.size()) return false;
  if (_out_sck_id >= m_out_socket->m_node->m_sf_node->m_out_types.size()) return false;

  if (m_inp_socket->m_node->m_sf_node->m_inp_types[_inp_sck_id].m_type !=
      m_out_socket->m_node->m_sf_node->m_out_types[_out_sck_id].m_type) return false;

  auto con_cnt = m_inp_socket->connections_count();
  if (!con_cnt) return false;
  if (*con_cnt != 0) return false;

  return true;

}

void qnode_connection_t::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mousePressEvent(event);

}

void qnode_connection_t::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mouseMoveEvent(event);

}
