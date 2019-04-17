#include <qppcad/ws_item/node_book/qnode.hpp>

using namespace qpp;
using namespace qpp::cad;


qnode_t::qnode_t() {

  m_pressed = false;
  setFlag(ItemIsMovable);

}

QRectF qnode_t::boundingRect() const {

  if (m_sflow_node) {
      int max_c = std::max(m_sflow_node->m_inp_types.size(), m_sflow_node->m_out_types.size());
      int new_height = 60 + max_c * m_pin_size;
      return QRectF(0, 0, m_width, new_height);
    }
  else {
    return QRectF(0, 0, m_width, m_height);
    }

}

void qnode_t::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {

  QRectF rect = boundingRect();
  //QRectF rect_m = rect.to
  painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addRoundedRect(rect, 4, 4);
  QPen pen(Qt::black, 3);
  painter->setPen(pen);
  painter->fillPath(path, Qt::gray);
  painter->drawPath(path);

  QString label =
      m_sflow_node ? QString::fromStdString(m_sflow_node->m_node_name) : tr("Unknown");
  QFontMetrics fm(painter->font());

  auto fmh125 = fm.height()*1.35;
  auto fmh01 = fm.height()*1.01;
  painter->drawText( QPoint(( m_width - fm.width(label)) / 2, fmh01), label);
  painter->drawLine(QLine(0, fmh125, m_width, fmh125));

  if (!m_sflow_node) return;

  int max_c = std::max(m_sflow_node->m_inp_types.size(), m_sflow_node->m_out_types.size());
  int new_height = 60 + max_c * m_pin_size;
  int h = new_height - fmh125;
  int link_padding = 10;
  int dh = 2 * m_pin_size * m_sflow_node->m_inp_types.size();
  int dm = (h - dh) / 2;

  for (size_t i = 0; i < m_sflow_node->m_inp_types.size(); i++) {
      //int dti = i >= 1 ? link_padding*(i) : 0 ;
      painter->drawEllipse(QRectF(5,
                                  fmh125 + dm +
                                  + m_pin_size * 2 * i,
                                  2*m_pin_size, 2*m_pin_size));
    }

}

void qnode_t::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  m_pressed = true;
  update();
  QGraphicsItem::mousePressEvent(event);

}

void qnode_t::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  m_pressed = false;
  update();
  QGraphicsItem::mouseReleaseEvent(event);

}
