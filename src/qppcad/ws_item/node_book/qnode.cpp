#include <qppcad/ws_item/node_book/qnode.hpp>
#include <QGraphicsScene>
#include <QApplication>

using namespace qpp;
using namespace qpp::cad;

qnode_t::qnode_t(QGraphicsItem *parent) : QGraphicsItem(parent)  {

  m_pressed = false;
  setFlag(ItemIsMovable);

}

qnode_t::~qnode_t() {

  m_input_nodes.clear();
  m_output_nodes.clear();

}

void qnode_t::set_sflow_node(std::shared_ptr<sflow_node_t> node) {

  m_sflow_node = node;
  m_input_nodes.clear();
  m_output_nodes.clear();

  QFontMetrics fm(QApplication::font());
  QRectF rect = boundingRect();

  auto fmh125 = fm.height()*1.35;
  auto fmh01 = fm.height()*0.9;

  int num_inps = m_sflow_node->m_inp_types.size();
  int h = rect.height();
  int h_a = h - fmh125;

  int l_p = 10;
  int dh_i = 2 * m_socket_size * num_inps + m_socket_spacing * (num_inps - 1);
  int dm_i = (h_a - dh_i) / 2;

  int num_outs = m_sflow_node->m_out_types.size();
  int dh_o = 2 * m_socket_size * num_outs + m_socket_spacing * (num_outs - 1);
  int dm_o = (h_a - dh_o) / 2;

  for (size_t i = 0; i < m_sflow_node->m_inp_types.size(); i++) {

      auto inp_sck = std::make_shared<qnode_socket_t>(this, m_socket_size, Qt::red);
      this->scene()->addItem(inp_sck.get());
      QPoint inp_sck_pos = {
        m_x_offset,
        fmh125 + dm_i + (m_socket_size * 2 + m_socket_spacing) * i
      };
      inp_sck->setPos(inp_sck_pos);

      m_input_nodes.push_back(inp_sck);

    }

  for (size_t i = 0; i < m_sflow_node->m_out_types.size(); i++) {

      auto out_sck = std::make_shared<qnode_socket_t>(this, m_socket_size, Qt::blue);
      this->scene()->addItem(out_sck.get());
      QPoint out_sck_pos = {
        m_width - 2*m_socket_size - m_x_offset,
        fmh125 + dm_o + (m_socket_size * 2 + m_socket_spacing) * i
      };
      out_sck->setPos(out_sck_pos);

      m_output_nodes.push_back(out_sck);

    }

}

QRectF qnode_t::boundingRect() const {

  if (m_sflow_node) {
      int max_c = std::max(m_sflow_node->m_inp_types.size(), m_sflow_node->m_out_types.size());
      int new_height = 60 + max_c * 2 * m_socket_size + (max_c - 1) * m_socket_spacing;
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
  QPen pen(Qt::black, 2);
  painter->setPen(pen);
  painter->fillPath(path, m_node_bg_color);
  painter->drawPath(path);

  QString label =
      m_sflow_node ? QString::fromStdString(m_sflow_node->m_node_name) : QObject::tr("Unknown");
  QFontMetrics fm(painter->font());

  auto fmh125 = fm.height()*1.35;
  auto fmh01 = fm.height()*0.9;
  painter->drawLine(QLine(0, fmh125, m_width, fmh125));

  QPen pen_label(m_node_label_color, 1);
  painter->setPen(pen_label);
  painter->drawText( QPoint(( m_width - fm.width(label)) / 2, fmh01), label);


  if (!m_sflow_node) return;

  int num_inps = m_sflow_node->m_inp_types.size();
  int h = rect.height();
  int h_a = h - fmh125;

  int l_p = 10;
  int dh_i = 2 * m_socket_size * num_inps + m_socket_spacing * (num_inps - 1);
  int dm_i = (h_a - dh_i) / 2;

  //draw inps
//  QPen pen_inps(Qt::darkRed, 3);
//  QBrush brsh_inps(Qt::black);
//  painter->setPen(pen_inps);
//  painter->setBrush(brsh_inps);
//  for (size_t i = 0; i < num_inps; i++) {
//      //int dti = i >= 1 ? link_padding*(i) : 0 ;
//      painter->drawEllipse(QRectF(m_x_offset,
//                                  fmh125 + dm_i +
//                                  + (m_pin_size * 2 + m_pin_spacing) * i,
//                                  2*m_pin_size, 2*m_pin_size));
//    }

  //draw outs
//  int num_outs = m_sflow_node->m_out_types.size();
//  int dh_o = 2 * m_pin_size * num_outs + m_pin_spacing * (num_outs - 1);
//  int dm_o = (h_a - dh_o) / 2;
//  QPen pen_outs(Qt::darkGreen, 3);
//  QBrush brsh_outs(Qt::black);
//  painter->setPen(pen_outs);
//  painter->setBrush(brsh_outs);
//  for (size_t i = 0; i < num_outs; i++) {
//      //int dti = i >= 1 ? link_padding*(i) : 0 ;
//      painter->drawEllipse(QRectF(m_width - 2*m_pin_size - m_x_offset,
//                                  fmh125 + dm_o +
//                                  + (m_pin_size * 2 + m_pin_spacing) * i,
//                                  2*m_pin_size, 2*m_pin_size));
//    }

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
