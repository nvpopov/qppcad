#ifndef QPP_CAD_NODE_BOOK_GRAPHICS_VIEW
#define QPP_CAD_NODE_BOOK_GRAPHICS_VIEW

#include <qppcad/qppcad.hpp>
#include <QGraphicsView>
#include <QPoint>
#include <QEvent>
#include <QWheelEvent>

namespace qpp {

  namespace cad {

    //https://github.com/rochus/qt5-node-editor
    class node_book_graphics_view_t : public QGraphicsView {

      Q_OBJECT

      public:

        bool m_drag_view{false};

        explicit node_book_graphics_view_t(QWidget *parent = nullptr);
        node_book_graphics_view_t(QGraphicsScene *scene, QWidget *parent = nullptr);

      protected:

        void wheelEvent(QWheelEvent *event) override ;
        void mouseMoveEvent(QMouseEvent *event) override ;
        void mousePressEvent(QMouseEvent *event) override ;
        void mouseReleaseEvent(QMouseEvent *event) override ;
        void resizeEvent(QResizeEvent *event) override ;

    };

  }

}

#endif
