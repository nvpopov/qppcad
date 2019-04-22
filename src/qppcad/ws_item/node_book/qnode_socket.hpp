#ifndef QPPCAD_QNODE_SOCKET
#define QPPCAD_QNODE_SOCKET

#include <qppcad/qppcad.hpp>
#include <QGraphicsPathItem>
#include <QColor>

namespace qpp {

  namespace cad {

    class qnode_t;
    class qnode_connection_t;

    class qnode_socket_t : public QGraphicsPathItem {

      public:

        enum { Type = UserType + 33 };

        int m_socket_radius{6};
        bool m_is_inp_socket{false};
        size_t m_socket_id;
        QColor m_socket_color;

        qnode_t *m_node{nullptr};
        qnode_connection_t* m_connection{nullptr};

        qnode_socket_t(QGraphicsItem *parent = 0,
                       int _socket_radius = 4,
                       QColor _socket_color = Qt::red);
        ~qnode_socket_t();

        opt<size_t> connections_count();
        int type() const override;

      protected:

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    }; // class qnode_socket_t

  } // namespace cad

} // namespace qpp

#endif
