#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

qnode_connection_t::qnode_connection_t(QGraphicsItem *parent) :
  QGraphicsPathItem(parent) {

}

qnode_connection_t::~qnode_connection_t() {

}

void qnode_connection_t::update_path() {

  if (!m_node_0 || !m_node_1) {
      return;
    }



}

void qnode_connection_t::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mousePressEvent(event);

}

void qnode_connection_t::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mouseMoveEvent(event);

}
