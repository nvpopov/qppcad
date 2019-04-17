#ifndef QPPCAD_NODE_BOOK_QNODE
#define QPPCAD_NODE_BOOK_QNODE

#include <qppcad/qppcad.hpp>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    class qnode :public QGraphicsItem {

      public:

        std::shared_ptr<sflow_node_t> m_sflow_node{nullptr};

        qnode();
        QRectF boundingRect() const;

        // overriding paint()
        void paint(QPainter * painter,
                   const QStyleOptionGraphicsItem * option,
                   QWidget * widget);

        // item state
        bool m_pressed;

      protected:
        // overriding mouse events
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    };

  } // namespace cad

} // namespace qpp


#endif
