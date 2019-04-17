#include <qppcad/ws_item/node_book/qnode.hpp>

using namespace qpp;
using namespace qpp::cad;


qnode::qnode() {

  m_pressed = false;
  setFlag(ItemIsMovable);

}

QRectF qnode::boundingRect() const {

  return QRectF(0,0,180,140);

}

void qnode::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {

  QRectF rect = boundingRect();
  //QRectF rect_m = rect.to
  //painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addRoundedRect(rect, 4, 4);
  QPen pen(Qt::black, 4);
  painter->setPen(pen);
  painter->fillPath(path, Qt::gray);
  painter->drawPath(path);

}

void qnode::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  m_pressed = true;
  update();
  QGraphicsItem::mousePressEvent(event);

}

void qnode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  m_pressed = false;
  update();
  QGraphicsItem::mouseReleaseEvent(event);

}
