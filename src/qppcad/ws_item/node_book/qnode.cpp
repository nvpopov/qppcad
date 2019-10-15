#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/node_book/qnode.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <qppcad/ws_item/node_book/qnode_socket_colorize.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/sflow/sflow_parameter_ws_item.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>

#include <QGraphicsScene>
#include <QApplication>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

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

void qnode_t::updated_externally(uint32_t update_reason) {

  if (!m_scene) return;
  if (!m_scene->m_parent_node_book) return;

  // make sflow node dirty
  if (m_sf_node) {

    }

  m_scene->m_parent_node_book->updated_externally(update_reason);

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

  for (size_t i = 0; i < m_sf_node->m_ipl_types.size(); i++)

    if (m_sf_node->m_ipl.size() > i && m_sf_node->m_ipl[i]) {

        QWidget *_inpl_widget{nullptr};
        //iupdatable_t *master_item{nullptr};

        //if (m_scene && m_scene->m_parent_node_book) master_item = m_scene->m_parent_node_book;

        switch (m_sf_node->m_ipl_types[i].m_type) {

          case sflow_parameter_e::sfpar_int : {

              qbinded_int_spinbox_t *b_sb = new qbinded_int_spinbox_t;
              b_sb->set_min_max_step(-100000, 100000, 1);
              sflow_parameter_int_t *sf_par_int =
                  m_sf_node->m_ipl[i]->cast_as<sflow_parameter_int_t>();

              if (sf_par_int) {

                  b_sb->bind_value(&sf_par_int->m_value, this);
                  b_sb->m_updated_externally_event = true;
                  b_sb->m_upd_flag = ws_item_updf_regenerate_content;

                  b_sb->setFixedWidth(astate->size_guide.node_book_inplace_par_width());
                  _inpl_widget = b_sb;
                  m_inplace_wdgts.push_back(b_sb);

                }

              break;

            }

          case sflow_parameter_e::sfpar_float : {

              qbinded_float_spinbox_t *b_sb = new qbinded_float_spinbox_t;
              b_sb->set_min_max_step(-100000, 100000, 0.001, 3);
              //b_sb->setFixedWidth(170);

              sflow_parameter_float_t *sf_par_float =
                  m_sf_node->m_ipl[i]->cast_as<sflow_parameter_float_t>();

              if (sf_par_float) {

                  b_sb->bind_value(&sf_par_float->m_value,  this);
                  b_sb->m_updated_externally_event = true;
                  b_sb->m_upd_flag = ws_item_updf_regenerate_content;

                  b_sb->setFixedWidth(astate->size_guide.node_book_inplace_par_width());
                  _inpl_widget = b_sb;
                  m_inplace_wdgts.push_back(b_sb);

                }

              break;

            }

          case sflow_parameter_e::sfpar_v3f : {

              qbinded_float3_input_t *b_v3f = new qbinded_float3_input_t;
              b_v3f->set_min_max_step(-10000, 10000, 0.01);

              sflow_parameter_v3f_t *sf_par_v3f =
                  m_sf_node->m_ipl[i]->cast_as<sflow_parameter_v3f_t>();

              if (sf_par_v3f) {

                  b_v3f->bind_value(&sf_par_v3f->m_value, this);
                  b_v3f->m_updated_externally_event = true;
                  b_v3f->m_upd_flag = ws_item_updf_regenerate_content;

                  //b_sb->setFixedWidth(astate->size_guide.node_book_inplace_par_width());
                  _inpl_widget = b_v3f;
                  m_inplace_wdgts.push_back(b_v3f);

                }

              break;

            }

          case sflow_parameter_e::sfpar_ws_item : {

              qbinded_ws_item_combobox_t *b_wsc = new qbinded_ws_item_combobox_t;
              b_wsc->m_type_id = geom_view_t::get_type_static();

              sflow_parameter_ws_item_t *sf_par_wsi =
                  m_sf_node->m_ipl[i]->cast_as<sflow_parameter_ws_item_t>();

              if (sf_par_wsi) {

                  b_wsc->bind_value(&sf_par_wsi->m_value,
                                    this,
                                    m_scene->m_parent_node_book->m_parent_ws);

                  b_wsc->m_updated_externally_event = true;
                  b_wsc->m_upd_flag = ws_item_updf_regenerate_content;

                  //b_sb->setFixedWidth(astate->size_guide.node_book_inplace_par_width());
                  _inpl_widget = b_wsc;
                  m_inplace_wdgts.push_back(b_wsc);

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

        if (!m_sf_node->m_ipl_types[i].m_editable && _inpl_widget)
          _inpl_widget->setEnabled(false);

        if (_inpl_widget) m_inplace_wdgt_lt->addRow(
              QString::fromStdString(m_sf_node->m_ipl_types[i].m_name),
              _inpl_widget);

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

  if (node->m_front_end_width) m_width = *(node->m_front_end_width);

  if (m_sf_node->has_ipls()) {
      m_sf_node->explicit_create_ipl();
      construct_inplace_widgets();
    }

  QRectF rect = boundingRect();
  int h = rect.height() - m_label_height;

  bool is_single_node = m_sf_node->is_single_node();
  if (m_inplace_wdgt && !is_single_node) h -= m_inplace_wdgt->height();

  m_dh_i = 2 * m_socket_size * num_inps() + m_socket_spacing * (num_inps() - 1);
  m_dm_i = (h - m_dh_i) / 2;

  m_dh_o = 2 * m_socket_size * num_outs() + m_socket_spacing * (num_outs() - 1);
  m_dm_o = (h - m_dh_o) / 2;

  if (is_single_node) {
      m_dm_i = m_inplace_wdgt->height() + m_socket_size / 2;
      m_dm_o = m_inplace_wdgt->height() + m_socket_size / 2;
    }

  for (size_t i = 0; i < m_sf_node->m_inp_types.size(); i++) {

      auto inp_sck = std::make_shared<qnode_socket_t>(
                       this,
                       m_socket_size,
                       sck_colorize_helper::get_color(m_sf_node->m_inp_types[i].m_type));

      QPoint inp_sck_pos = {
        m_x_offset,
        m_label_height + m_dm_i + (m_socket_size * 2 + m_socket_spacing) * static_cast<int>(i)
      };

      inp_sck->setPos(inp_sck_pos);
      inp_sck->m_node = shared_from_this();
      inp_sck->m_socket_id = i;
      inp_sck->m_is_inp_socket = true;
      m_inp_sockets.push_back(inp_sck);

    }

  for (size_t i = 0; i < m_sf_node->m_out_types.size(); i++) {

      auto out_sck = std::make_shared<qnode_socket_t>(
                       this,
                       m_socket_size,
                       sck_colorize_helper::get_color(m_sf_node->m_out_types[i].m_type));

      QPoint out_sck_pos = {
        m_width - 2 * m_socket_size - m_x_offset,
        m_label_height + m_dm_o + (m_socket_size * 2 + m_socket_spacing) * static_cast<int>(i)
      };

      out_sck->setPos(out_sck_pos);
      out_sck->m_node = shared_from_this();
      out_sck->m_socket_id = i;
      out_sck->m_is_inp_socket = false;

      m_out_sockets.push_back(out_sck);

    }

}

QRectF qnode_t::boundingRect() const {

  if (!m_sf_node) return QRectF(0, 0, m_width, m_height);

  int additional_h_from_inplace = 0;
  if (m_inplace_wdgt) additional_h_from_inplace = m_inplace_wdgt->height();

  if (m_sf_node->is_single_node())
    return QRectF(0, 0, m_width, m_label_height + additional_h_from_inplace + 10);

  int max_c = std::max(m_sf_node->m_inp_types.size(), m_sf_node->m_out_types.size());
  int new_height = m_label_height
                   + max_c * 4 * m_socket_size + (max_c - 1) * m_socket_spacing
                   + additional_h_from_inplace;

  return QRectF(0, 0, m_width, new_height);

}

void qnode_t::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {

  if (!m_sf_node) return;

  QRectF rect = boundingRect();

  QFontMetrics fm(painter->font());

  bool is_single_node = m_sf_node->is_single_node();

  // draw stuff

  // draw node back
  painter->setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addRoundedRect(rect, 4, 4);
  QPen pen(Qt::black, 2);
  painter->setPen(pen);
  painter->fillPath(path, m_node_bg_color);
  painter->drawPath(path);

  // draw label separator
  painter->drawLine(QLine(0, m_label_height, m_width, m_label_height));

  // draw label text
  QPen pen_label(m_node_label_color, 1);
  painter->setPen(pen_label);
  painter->drawText(QRect(1, 1, m_width - 1, m_label_height - 1), m_node_name, m_label_text_opt);

  //draw sockets names
  if (is_single_node) return;

  for (size_t i = 0; i < m_sf_node->m_inp_types.size(); i++) {

      QString _pin_name = QString::fromStdString(m_sf_node->m_inp_types[i].m_sck_name);

      QPoint inp_sck_pos = {
        5,
        static_cast<int>(m_label_height + m_dm_i + (m_socket_size * 2 + m_socket_spacing)
        * static_cast<int>(i) + fm.height() * 0.55)
      };

      painter->drawText(inp_sck_pos, _pin_name);

    }

  for (size_t i = 0; i < m_sf_node->m_out_types.size(); i++) {

      QString _pin_name = QString::fromStdString(m_sf_node->m_out_types[i].m_sck_name);

      QPoint out_sck_pos = {
        m_width - 5 - fm.width(_pin_name),
        static_cast<int>(m_label_height + m_dm_o + (m_socket_size * 2 + m_socket_spacing)
        * static_cast<int>(i) + fm.height() * 0.55)
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

QVariant qnode_t::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) {

  if (change == ItemPositionChange && scene()) {
      m_scene->update_connections_with_node(shared_from_this());
    }

  return QGraphicsItem::itemChange(change, value);

}

