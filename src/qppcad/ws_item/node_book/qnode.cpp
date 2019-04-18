#include <qppcad/ws_item/node_book/qnode.hpp>

using namespace qpp;
using namespace qpp::cad;


qnode_t::qnode_t(QGraphicsItem *parent) : QGraphicsItem(parent)  {

  m_pressed = false;
  setFlag(ItemIsMovable);

}

qnode_t::~qnode_t() {

}

void qnode_t::set_sflow_node(std::shared_ptr<sflow_node_t> node) {

  m_sflow_node = node;

}

QRectF qnode_t::boundingRect() const {

  if (m_sflow_node) {
      int max_c = std::max(m_sflow_node->m_inp_types.size(), m_sflow_node->m_out_types.size());
      int new_height = 60 + max_c * 2 * m_pin_size + (max_c - 1) * m_pin_spacing;
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
      m_sflow_node ? QString::fromStdString(m_sflow_node->m_node_name) : QObject::tr("Unknown");
  QFontMetrics fm(painter->font());

  auto fmh125 = fm.height()*1.35;
  auto fmh01 = fm.height()*1.01;
  painter->drawText( QPoint(( m_width - fm.width(label)) / 2, fmh01), label);
  painter->drawLine(QLine(0, fmh125, m_width, fmh125));

  if (!m_sflow_node) return;

  int num_inps = m_sflow_node->m_inp_types.size();
  int h = rect.height();
  int h_a = h - fmh125;

  int l_p = 10;
  int dh_i = 2 * m_pin_size * num_inps + m_pin_spacing * (num_inps - 1);
  int dm_i = (h_a - dh_i) / 2;

  //draw inps
  QPen pen_inps(Qt::darkRed, 3);
  QBrush brsh_inps(Qt::black);
  painter->setPen(pen_inps);
  painter->setBrush(brsh_inps);
  for (size_t i = 0; i < num_inps; i++) {
      //int dti = i >= 1 ? link_padding*(i) : 0 ;
      painter->drawEllipse(QRectF(m_x_offset,
                                  fmh125 + dm_i +
                                  + (m_pin_size * 2 + m_pin_spacing) * i,
                                  2*m_pin_size, 2*m_pin_size));
    }

  //draw outs
  int num_outs = m_sflow_node->m_out_types.size();
  int dh_o = 2 * m_pin_size * num_outs + m_pin_spacing * (num_outs - 1);
  int dm_o = (h_a - dh_o) / 2;
  QPen pen_outs(Qt::darkGreen, 3);
  QBrush brsh_outs(Qt::black);
  painter->setPen(pen_outs);
  painter->setBrush(brsh_outs);
  for (size_t i = 0; i < num_outs; i++) {
      //int dti = i >= 1 ? link_padding*(i) : 0 ;
      painter->drawEllipse(QRectF(m_width - 2*m_pin_size - m_x_offset,
                                  fmh125 + dm_o +
                                  + (m_pin_size * 2 + m_pin_spacing) * i,
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
