#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <QPainter>

using namespace qpp;
using namespace qpp::cad;

qnode_socket_t::qnode_socket_t(QGraphicsItem *parent,
                               int _socket_radius,
                               QColor _socket_color)
  : QGraphicsPathItem(parent), m_socket_radius(_socket_radius), m_socket_color(_socket_color) {

  QPainterPath p;

  p.addRect(0, 0, 2*m_socket_radius, 2*m_socket_radius);
  setPath(p);
  setPen(QPen(m_socket_color, 2));
  setBrush(Qt::black);

  setFlag(QGraphicsItem::ItemIsMovable, false);

}

qnode_socket_t::~qnode_socket_t() {

}
