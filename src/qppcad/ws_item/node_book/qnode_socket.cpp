#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <qppcad/ws_item/node_book/qnode_connection.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <qppcad/ws_item/node_book/node_book.hpp>
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

  p.addRect(0, 0, 2 * m_socket_radius, 2 * m_socket_radius);
  setPath(p);
  setPen(QPen(m_socket_color, 2));
  setBrush(Qt::black);

  setFlag(QGraphicsItem::ItemIsMovable, false);
  setAcceptHoverEvents(true);
  setZValue(200);

}

qnode_socket_t::~qnode_socket_t() {

}

opt<size_t> qnode_socket_t::connections_count() {

  if (!m_node) return std::nullopt;
  if (!m_node->m_sf_node) return std::nullopt;

  size_t ret_con_count = std::count_if(std::begin(m_node->m_scene->m_connections),
                                       std::end(m_node->m_scene->m_connections),
                                       [this](std::shared_ptr<qnode_connection_t> con)
                                       {return con && con->m_inp_socket &&
                                        con->m_out_socket && con->m_inp_socket == this;});

  return opt<size_t>(ret_con_count);

}

int qnode_socket_t::type() const {
  return Type;
}

void qnode_socket_t::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mousePressEvent(event);
  event->accept();

  if (!m_connection) {
      m_connection = std::make_shared<qnode_connection_t>();
      scene()->addItem(m_connection.get());

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

  QGraphicsItem::mouseMoveEvent(event);

  if (m_connection) {
      m_connection->update_path(event->scenePos());
    }

}

void qnode_socket_t::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mouseReleaseEvent(event);

  if (m_connection) {

      QList<QGraphicsItem *> items = scene()->items(event->scenePos());
      QGraphicsItem *item{nullptr};

      for (auto _item : items) {
          if (_item && _item->type() == qnode_socket_t::Type)
            item = _item;
        }

      bool delete_item = true;

      if (item) {

          qnode_socket_t* item_c = qgraphicsitem_cast<qnode_socket_t*>(item);

          if (item_c && item_c != this && m_is_inp_socket != item_c->m_is_inp_socket) {

              //determine who is who
              //this is inp, target in out
              if (m_is_inp_socket && !item_c->m_is_inp_socket) {
                  m_connection->m_inp_socket = this;
                  m_connection->m_out_socket = item_c;
                }

              if (!m_is_inp_socket && item_c->m_is_inp_socket) {
                  m_connection->m_inp_socket = item_c;
                  m_connection->m_out_socket = this;
                }

              if (m_connection->is_connection_valid()) {
                  m_connection->update_path(event->scenePos(), true);
                  delete_item = false;
                  m_node->m_scene->m_connections.push_back(m_connection);
                  m_connection = nullptr;
                  if (m_node && m_node->m_scene && m_node->m_scene->m_parent_node_book)
                    m_node->m_scene->m_parent_node_book->execute();
                }

            } // if (item_c && item_c != this)

        } // if (item)

      if (delete_item) {
          scene()->removeItem(m_connection.get());
          m_connection.reset();
          m_connection = nullptr;
          return;
        }

      //m_connection = nullptr;

    } // if (m_connection)

}
