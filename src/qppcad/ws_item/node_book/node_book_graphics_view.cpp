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

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setResizeAnchor(NoAnchor);
  setTransformationAnchor(AnchorUnderMouse);

}

QRectF node_book_graphics_view_t::get_visible_rect() {

//  QPointF a = mapToScene(QPoint(0, 0));
//  QPointF b = mapToScene(QPoint(viewport()->width(), viewport()->height()));
//
//  return QRectF(a, b);
  return viewport()->geometry();

}

void node_book_graphics_view_t::wheelEvent(QWheelEvent *event) {

  //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  if (event && event->modifiers() == Qt::ControlModifier) {

      double scaleFactor = 1.05;
      if (event->delta() > 0) scale(scaleFactor, scaleFactor);
      else scale(1.0 / scaleFactor, 1.0 / scaleFactor);
      event->accept();
      return;

    }

  if (QGraphicsItem *item = itemAt(event->pos()); item != nullptr) {

      QGraphicsView::wheelEvent(event);

    }

}

void node_book_graphics_view_t::mouseMoveEvent(QMouseEvent *event) {

  viewport()->setCursor(Qt::ArrowCursor);

  if (m_drag_view) {
      //translate(1, 1);
    }
  QGraphicsView::mouseMoveEvent(event);

}

void node_book_graphics_view_t::mousePressEvent(QMouseEvent *event) {

  viewport()->setCursor(Qt::ArrowCursor);

  if (event->button() == Qt::LeftButton) {
      m_drag_view = true;
      setDragMode(QGraphicsView::ScrollHandDrag);
      viewport()->setCursor(Qt::ArrowCursor);
    }

  event->accept();
  QGraphicsView::mousePressEvent(event);

}

void node_book_graphics_view_t::mouseReleaseEvent(QMouseEvent *event) {

  viewport()->setCursor(Qt::ArrowCursor);

  if (event->button() == Qt::LeftButton) {
      m_drag_view = false;
      setDragMode(QGraphicsView::NoDrag);
    }
  event->accept();
  QGraphicsView::mouseReleaseEvent(event);

}

void node_book_graphics_view_t::resizeEvent(QResizeEvent *event) {

//  static bool first_resize = true;

//  if (first_resize) {
//    centerOn(width()/2 - 50, height()/2 - 50);
//    first_resize = false;
//  }

  QGraphicsView::resizeEvent(event);

}

void node_book_graphics_view_t::mouseDoubleClickEvent(QMouseEvent *event) {

  //assert(false);
  QGraphicsView::mouseDoubleClickEvent(event);

}
