#ifndef QPPCAD_QNODE_SOCKET
#define QPPCAD_QNODE_SOCKET

#include <qppcad/qppcad.hpp>
#include <QGraphicsPathItem>
#include <QColor>

namespace qpp {

  namespace cad {

    class qnode_socket_t : public QGraphicsPathItem {

      public:

        int m_socket_radius{6};
        QColor m_socket_color;

        qnode_socket_t(QGraphicsItem *parent = 0,
                       int _socket_radius = 4,
                       QColor _socket_color = Qt::red);
        ~qnode_socket_t();

    };

  }

}

#endif
