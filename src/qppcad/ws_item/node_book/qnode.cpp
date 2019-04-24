#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <QGraphicsScene>
#include <QApplication>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_item/node_book/qnode_socket_colorize.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>

using namespace qpp;
using namespace qpp::cad;

qnode_t::qnode_t(QGraphicsItem *parent) : QGraphicsItem(parent)  {

  m_pressed = false;
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  setFlag(QGraphicsItem::ItemIsSelectable);

  m_label_text_opt.setAlignment(Qt::AlignCenter);

}

qnode_t::~qnode_t() {

  m_inp_sockets.clear();
  m_out_sockets.clear();

}

void qnode_t::construct_inplace_widgets() {

  app_state_t *astate = app_state_t::get_inst();

  m_inplace_wdgt = new QWidget;
  m_inplace_wdgt_lt = new QFormLayout;

  m_inplace_wdgt->setFixedWidth(m_width);
  m_inplace_wdgt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  m_inplace_wdgt->setLayout(m_inplace_wdgt_lt);
  m_inplace_wdgt->setPalette(Qt::transparent);

  m_inplace_pars_widget = new QGraphicsProxyWidget(this);
  m_inplace_pars_widget->setWidget(m_inplace_wdgt);

  for (size_t i = 0; i < m_sf_node->m_inplace_types.size(); i++)

    if (m_sf_node->m_inplace_parameters.size() > i && m_sf_node->m_inplace_parameters[i]) {

        QWidget *_inpl_widget{nullptr};

        switch (m_sf_node->m_inplace_types[i].m_type) {

          case sflow_parameter_e::sfpar_int : {
              //astate->tlog("NB_DEBUG trying to create inplace widget i = {}", i);
              qbinded_int_spinbox_t *b_sb = new qbinded_int_spinbox_t;
              sflow_parameter_int_t *sf_par_int =
                  m_sf_node->m_inplace_parameters[i]->cast_as<sflow_parameter_int_t>();
              if (sf_par_int) {
                  b_sb->bind_value(&sf_par_int->m_value);
                  _inpl_widget = b_sb;
                  m_inplace_wdgts.push_back(b_sb);
                }
              break;
            }

          case sflow_parameter_e::sfpar_float : {
              qbinded_float_spinbox_t *b_sb = new qbinded_float_spinbox_t;
              sflow_parameter_float_t *sf_par_int =
                  m_sf_node->m_inplace_parameters[i]->cast_as<sflow_parameter_float_t>();
              if (sf_par_int) {
                  b_sb->bind_value(&sf_par_int->m_value);
                  _inpl_widget = b_sb;
                  m_inplace_wdgts.push_back(b_sb);
                }
              break;
            }

          case sflow_parameter_e::sfpar_bool : {
              break;
            }

          default : {
              break;
            }

          }

        if (!m_sf_node->m_inplace_types[i].m_editable && _inpl_widget)
          _inpl_widget->setEnabled(false);

        if (_inpl_widget) m_inplace_wdgt_lt->addRow(
              QString::fromStdString(m_sf_node->m_inplace_types[i].m_name),_inpl_widget);
      }

  m_inplace_pars_widget->setPos(
        QPointF(0, boundingRect().height() - m_inplace_wdgt->height() - 5 ));

}

int qnode_t::type() const {
  return Type;
}

size_t qnode_t::num_inps() {

  if (m_sf_node) return m_sf_node->m_inp_types.size();
  return 0;

}

size_t qnode_t::num_outs() {

  if (m_sf_node) return m_sf_node->m_out_types.size();
  return 0;

}

void qnode_t::set_sflow_node(std::shared_ptr<sflow_node_t> node) {

  m_sf_node = node;
  m_inp_sockets.clear();
  m_out_sockets.clear();

  m_node_name =
      m_sf_node ? QString::fromStdString(m_sf_node->m_node_name) : QObject::tr("Unknown");

  if (m_sf_node->validate_inplace_parameters()) construct_inplace_widgets();

  QRectF rect = boundingRect();
  int h = rect.height();

  bool is_single_node = m_sf_node->is_single_node();
  if (m_inplace_wdgt && !is_single_node) h -= m_inplace_wdgt->height();

  int h_a = h - m_label_height;

  int l_p = 10;
  int dh_i = 2 * m_socket_size * num_inps() + m_socket_spacing * (num_inps() - 1);
  int dm_i = (h_a - dh_i) / 2;

  int dh_o = 2 * m_socket_size * num_outs() + m_socket_spacing * (num_outs() - 1);
  int dm_o = (h_a - dh_o) / 2;

  for (size_t i = 0; i < m_sf_node->m_inp_types.size(); i++) {

      auto inp_sck = new qnode_socket_t(
            this,
            m_socket_size,
            sck_colorize_helper::get_color(m_sf_node->m_inp_types[i].m_type));

      QPoint inp_sck_pos = {
        m_x_offset,
        m_label_height + dm_i + (m_socket_size * 2 + m_socket_spacing) * i
      };

      inp_sck->setPos(inp_sck_pos);
      inp_sck->m_node = this;
      inp_sck->m_socket_id = i;
      inp_sck->m_is_inp_socket = true;
      m_inp_sockets.push_back(inp_sck);
      m_scene->m_sockets.push_back(inp_sck);

    }

  for (size_t i = 0; i < m_sf_node->m_out_types.size(); i++) {

      auto out_sck = new qnode_socket_t(
            this,
            m_socket_size,
            sck_colorize_helper::get_color(m_sf_node->m_out_types[i].m_type));

      QPoint out_sck_pos = {
        m_width - 2*m_socket_size - m_x_offset,
        m_label_height + dm_o + (m_socket_size * 2 + m_socket_spacing) * i
      };

      out_sck->setPos(out_sck_pos);
      out_sck->m_node = this;
      out_sck->m_socket_id = i;
      out_sck->m_is_inp_socket = false;

      m_out_sockets.push_back(out_sck);
      m_scene->m_sockets.push_back(out_sck);

    }

}

QRectF qnode_t::boundingRect() const {

  if (!m_sf_node) return QRectF(0, 0, m_width, m_height);

  if (m_sf_node->is_single_node())
    return QRectF(0, 0, m_width, m_label_height +
                  (m_inplace_wdgt != nullptr ? m_inplace_wdgt->height() : 0));

  int max_c = std::max(m_sf_node->m_inp_types.size(), m_sf_node->m_out_types.size());
  int new_height = m_label_height
      + max_c * 2 * m_socket_size + (max_c - 1) * m_socket_spacing
      + (m_inplace_wdgt != nullptr ? m_inplace_wdgt->height() : 0);

  return QRectF(0, 0, m_width, new_height);

}

void qnode_t::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {

  if (!m_sf_node) return;

  QRectF rect = boundingRect();
  int h = rect.height();

  QFontMetrics fm(painter->font());

  bool is_single_node = m_sf_node->is_single_node();
  if (m_inplace_wdgt && !is_single_node) h -= m_inplace_wdgt->height();

  int num_inps = m_sf_node->m_inp_types.size();
  int h_a = h - m_label_height;

  int l_p = 10;
  int dh_i = 2 * m_socket_size * num_inps + m_socket_spacing * (num_inps - 1);
  int dm_i = (h_a - dh_i) / 2;

  int num_outs = m_sf_node->m_out_types.size();
  int dh_o = 2 * m_socket_size * num_outs + m_socket_spacing * (num_outs - 1);
  int dm_o = (h_a - dh_o) / 2;

  // draw stuff

  painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addRoundedRect(rect, 4, 4);
  QPen pen(Qt::black, 2);
  painter->setPen(pen);
  painter->fillPath(path, m_node_bg_color);
  painter->drawPath(path);

  painter->drawLine(QLine(0, m_label_height, m_width, m_label_height));

  QPen pen_label(m_node_label_color, 1);
  painter->setPen(pen_label);
  painter->drawText(QRect(1, 1, m_width - 1, m_label_height - 1), m_node_name, m_label_text_opt);

  if (is_single_node) return;

  for (size_t i = 0; i < m_sf_node->m_inp_types.size(); i++) {
      QString _pin_name = QString::fromStdString(m_sf_node->m_inp_types[i].m_pin_name);
      QPoint inp_sck_pos = {
        5,
        m_label_height + dm_i + (m_socket_size * 2 + m_socket_spacing) * i + fm.height() * 0.37
      };
      painter->drawText(inp_sck_pos, _pin_name);
    }

  for (size_t i = 0; i < m_sf_node->m_out_types.size(); i++) {
      QString _pin_name = QString::fromStdString(m_sf_node->m_out_types[i].m_pin_name);
      QPoint out_sck_pos = {
        m_width - 5 - fm.width(_pin_name),
        m_label_height + dm_o + (m_socket_size * 2 + m_socket_spacing) * i + fm.height() * 0.37
      };
      painter->drawText(out_sck_pos, _pin_name);
    }

}

void qnode_t::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  scene()->clearSelection();
  setSelected(true);
  m_pressed = true;
  update();
  QGraphicsItem::mousePressEvent(event);

}

void qnode_t::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  m_pressed = false;
  update();
  QGraphicsItem::mouseReleaseEvent(event);

}

QVariant qnode_t::itemChange(QGraphicsItem::GraphicsItemChange change,
                             const QVariant &value) {

  app_state_t *astate = app_state_t::get_inst();

  if (change == ItemPositionChange && scene()) {
      //astate->tlog("qnode_t::itemChange()");
      m_scene->update_connections_with_node(this);
    }

  return QGraphicsItem::itemChange(change, value);

}

//void qnode_t::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {

//  app_state_t *astate = app_state_t::get_inst();

//  QMenu menu;
//  QAction *remove_action = menu.addAction("Remove");
//  QAction *unlink_all_action = menu.addAction("Unlink all");
//  QAction *selected_action = menu.exec(event->screenPos());

//  //if (m_scene) m_scene->sendEvent(
//  if (selected_action == remove_action && m_scene) {
//      m_scene->removeItem(this);
//      astate->tlog("qnode_t::contextMenuEvent(), remove");
//    }

//}
