#ifndef QPPCAD_NODE_BOOK_QNODE
#define QPPCAD_NODE_BOOK_QNODE

#include <qppcad/core/qppcad.hpp>
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <qppcad/sflow/sflow_node.hpp>
#include <qppcad/ws_item/node_book/qnode_socket.hpp>
#include <QGraphicsProxyWidget>
#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTextOption>

namespace qpp {

  namespace cad {

    class node_book_graphics_scene_t;
    class qnode_t : public QGraphicsItem, public std::enable_shared_from_this<qnode_t> {

      public:

        enum { Type = UserType + 32 };

        node_book_graphics_scene_t *m_scene;

        int m_width{180};
        int m_height{140};
        int m_socket_size{8};
        int m_socket_spacing{8};
        int m_x_offset{-16};
        int m_label_height{32};
        int m_lp{10};

        int m_dh_i{0};
        int m_dm_i{0};
        int m_dh_o{0};
        int m_dm_o{0};

        QPen m_node_outline_pen;
        QPen m_text_pen;

        QPen m_node_border_pen;
        QPen m_node_label_pen;

        QTextOption m_label_text_opt;

        QColor m_node_bg_color{QColor::fromRgb(34, 34, 34)};
        QColor m_node_label_color{QColor::fromRgb(224, 224, 224)};

        QString m_node_name;

        std::vector<std::shared_ptr<qnode_socket_t>> m_inp_sockets;
        std::vector<std::shared_ptr<qnode_socket_t>> m_out_sockets;

        std::shared_ptr<sflow_node_t> m_sf_node{nullptr};

        QGraphicsProxyWidget *m_inplace_pars_widget{nullptr};
        QWidget *m_inplace_wdgt{nullptr};
        QFormLayout *m_inplace_wdgt_lt{nullptr};
        std::vector<QWidget*> m_inplace_wdgts;

        explicit qnode_t(QGraphicsItem *parent = nullptr);
        ~qnode_t();

        void construct_inplace_widgets();
        int type() const override;
        size_t num_inps();
        size_t num_outs();

        void set_sflow_node(std::shared_ptr<sflow_node_t> node);
        QRectF boundingRect() const override;

        // overriding paint()
        void paint(QPainter * painter,
                   const QStyleOptionGraphicsItem * option,
                   QWidget * widget) override;

        // item state
        bool m_pressed;

      protected:

        // overriding mouse events
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    };

  } // namespace cad

} // namespace qpp


#endif
