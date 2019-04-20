#ifndef QPPCAD_QNODE_CONNECTION
#define QPPCAD_QNODE_CONNECTION

#include <qppcad/qppcad.hpp>
#include <QGraphicsPathItem>
#include <QColor>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

namespace qpp {

  namespace cad {

    class qnode_t;

    class qnode_connection_t : public QGraphicsPathItem {

      public:

        qnode_t *m_node_0;
        qnode_t *m_node_1;

        opt<size_t> m_node_0_socket_id;
        opt<size_t> m_node_1_socket_id;

        bool m_temporary{false};

        qnode_connection_t(QGraphicsItem *parent = 0);
        ~qnode_connection_t();

        void update_path();

      protected:

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    };

  } // namespace cad

} // namespace qpp

#endif
