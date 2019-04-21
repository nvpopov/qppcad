#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <qppcad/app_state.hpp>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>

using namespace qpp;
using namespace qpp::cad;

qnode_socket_t::qnode_socket_t(QGraphicsItem *parent,
                               int _socket_radius,
                               QColor _socket_color)
  : QGraphicsPathItem(parent),
    m_socket_radius(_socket_radius),
    m_socket_color(_socket_color) {

  QPainterPath p;

  p.addRect(0, 0, 2*m_socket_radius, 2*m_socket_radius);
  setPath(p);
  setPen(QPen(m_socket_color, 2));
  setBrush(Qt::black);

  setFlag(QGraphicsItem::ItemIsMovable, false);
  setAcceptHoverEvents(true);
  setZValue(200);

}

qnode_socket_t::~qnode_socket_t() {

}

void qnode_socket_t::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  app_state_t *astate = app_state_t::get_inst();
  astate->tlog("qnode_socket_t::mousePressEvent()");
  QGraphicsItem::mousePressEvent(event);
  event->accept();

  if (!m_connection) {
      m_connection = new qnode_connection_t();
      scene()->addItem(m_connection);

      if (m_is_inp_socket) {
          m_connection->m_inp_socket = this;
          m_connection->m_out_socket = nullptr;
        }
      else {
          m_connection->m_inp_socket = nullptr;
          m_connection->m_out_socket = this;
        }

    }

}

void qnode_socket_t::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

  app_state_t *astate = app_state_t::get_inst();
  astate->tlog("qnode_socket_t::mouseMoveEvent()");
  QGraphicsItem::mouseMoveEvent(event);

  if (m_connection) {
      m_connection->update_path(event->scenePos());
    }

}

void qnode_socket_t::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  app_state_t *astate = app_state_t::get_inst();
  astate->tlog("qnode_socket_t::mouseReleaseEvent()");
  QGraphicsItem::mouseReleaseEvent(event);

  if (m_connection) {

      QGraphicsItem *item =
          scene()->itemAt(event->scenePos(), scene()->views().first()->transform());

      bool delete_item = true;

      if (item && item != this) {

          auto item_c = qgraphicsitem_cast<qnode_socket_t*>(item);

          if (item_c && item_c != this) {

              astate->tlog("CASTED!");
              //determine who is who
              //this is inp, target in out

              if (m_is_inp_socket && !item_c->m_is_inp_socket) {
                  m_connection->m_inp_socket = this;
                  m_connection->m_out_socket = item_c;
                } else
                if (!m_is_inp_socket && item_c->m_is_inp_socket) {
                  m_connection->m_inp_socket = item_c;
                  m_connection->m_out_socket = this;
                }

              if (!m_connection->m_out_socket || !m_connection->m_inp_socket) {
                  m_connection->setParentItem(nullptr);
                  m_connection = nullptr;
                } else {
                  astate->tlog("m_connection->update_path(event->scenePos(), true)");
                  m_connection->update_path(event->scenePos(), true);
                  delete_item = false;
                  m_node->m_scene->m_connections.push_back(m_connection);
                  m_connection = nullptr;
                }

            } // if (item_c && item_c != this)

        } // if (item)

      if (delete_item) {
          scene()->removeItem(m_connection);
          delete m_connection;
          m_connection = nullptr;
          return;
        }

      //m_connection = nullptr;

    } // if (m_connection)

}
