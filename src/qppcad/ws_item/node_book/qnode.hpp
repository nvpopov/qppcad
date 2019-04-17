#ifndef QPPCAD_NODE_BOOK_QNODE
#define QPPCAD_NODE_BOOK_QNODE

#include <qppcad/qppcad.hpp>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    class qnode_t : public QObject, public QGraphicsItem {

      Q_OBJECT

      public:

        int m_width{180};
        int m_height{140};
        int m_pin_size{10};

        std::shared_ptr<sflow_node_t> m_sflow_node{nullptr};

        qnode_t();
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
