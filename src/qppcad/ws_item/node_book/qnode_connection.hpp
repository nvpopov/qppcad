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
    class qnode_socket_t;

    class qnode_connection_t : public QGraphicsPathItem {

      public:

        enum { Type = UserType + 34 };

        qnode_socket_t *m_out_socket{nullptr};
        qnode_socket_t *m_inp_socket{nullptr};

        QColor m_connection_color{Qt::gray};

        bool m_temporary{false};


        qnode_connection_t(QGraphicsItem *parent = 0);
        ~qnode_connection_t();

        int type() const override;

        void update_path(QPointF point,
                         bool finalize = false);
        void clear_path();
        bool is_connection_valid();

      protected:

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    };

  } // namespace cad

} // namespace qpp

#endif
