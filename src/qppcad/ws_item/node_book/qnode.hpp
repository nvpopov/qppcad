#ifndef QPPCAD_NODE_BOOK_QNODE
#define QPPCAD_NODE_BOOK_QNODE

#include <qppcad/qppcad.hpp>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

  namespace cad {

    class qnode_t : public QGraphicsItem {

      public:

        int m_width{180};
        int m_height{140};
        int m_pin_size{8};
        int m_pin_spacing{8};
        int m_x_offset{6};

        QPen m_node_border_pen;
        QPen m_node_label_pen;

        QColor m_node_bg_color{QColor::fromRgb(34, 34, 34)};
        QColor m_node_label_color{QColor::fromRgb(224, 224, 224)};

        std::shared_ptr<sflow_node_t> m_sflow_node{nullptr};

        qnode_t(QGraphicsItem *parent = nullptr);
        ~qnode_t();

        void set_sflow_node(std::shared_ptr<sflow_node_t> node);
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
