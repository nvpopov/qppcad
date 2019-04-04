#include <qppcad/ws_item/node_book/node_book_graphics_view.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_graphics_view_t::node_book_graphics_view_t(QWidget *parent)
  : QGraphicsView (nullptr, parent) {

}

node_book_graphics_view_t::node_book_graphics_view_t(QGraphicsScene *scene, QWidget *parent)
  : QGraphicsView (scene, parent) {

  setRenderHints(QPainter::Antialiasing |
      QPainter::TextAntialiasing |
      QPainter::HighQualityAntialiasing |
      QPainter::SmoothPixmapTransform);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setResizeAnchor(NoAnchor);
  setTransformationAnchor(AnchorUnderMouse);

}

void node_book_graphics_view_t::wheelEvent(QWheelEvent *event) {

  if (event->modifiers() & Qt::ControlModifier) {
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.25;
    if (event->delta() < 0) scale(scaleFactor, scaleFactor);
    else scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    event->accept();
  }
  else {
    QGraphicsView::wheelEvent(event);
  }

}

void node_book_graphics_view_t::mouseMoveEvent(QMouseEvent *event) {

  QGraphicsView::mouseMoveEvent(event);

}

void node_book_graphics_view_t::mousePressEvent(QMouseEvent *event) {

  QGraphicsView::mousePressEvent(event);

}

void node_book_graphics_view_t::mouseReleaseEvent(QMouseEvent *event) {

  QGraphicsView::mouseReleaseEvent(event);

}

void node_book_graphics_view_t::resizeEvent(QResizeEvent *event) {

  static bool first_resize = true;

  if (first_resize) {
    centerOn(width()/2 - 50, height()/2 - 50);
    first_resize = false;
  }

  QGraphicsView::resizeEvent(event);

}
