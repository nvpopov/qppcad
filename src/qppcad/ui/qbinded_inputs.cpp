#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/app_state.hpp>
#include <QMouseEvent>
#include <QColorDialog>

using namespace qpp;
using namespace qpp::cad;

void qbinded_checkbox_t::load_value_ex() {

  if (m_binded_value) {
      if (*m_binded_value) setCheckState(Qt::Checked);
      else setCheckState(Qt::Unchecked);
    }

}

qbinded_checkbox_t::qbinded_checkbox_t(QWidget *parent) : QCheckBox (parent) {

  connect(this,
          static_cast<void(qbinded_checkbox_t::*)(int)>(&qbinded_checkbox_t::stateChanged),
          this,
          &qbinded_checkbox_t::check_state_changed);

}

void qbinded_checkbox_t::check_state_changed(int state) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = state == Qt::Checked;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_float_spinbox_t::load_value_ex() {

  if (m_binded_value) {
      setValue(*m_binded_value);
    }

}

void qbinded_float_spinbox_t::set_min_max_step(double new_min,
                                             double new_max,
                                             double new_step,
                                             int decimals) {
  setRange(new_min, new_max);
  setSingleStep(new_step);
  setDecimals(decimals);

}

qbinded_float_spinbox_t::qbinded_float_spinbox_t(QWidget *parent) : QDoubleSpinBox (parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setAlignment(Qt::AlignCenter);
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setLocale(QLocale::C);

  connect(
    this,
    static_cast<void(qbinded_float_spinbox_t::*)(double)>(&qbinded_float_spinbox_t::valueChanged),
    this,
    &qbinded_float_spinbox_t::value_changed
    );

}

void qbinded_float_spinbox_t::set_suffix(QString &new_suffix) {

  setSuffix(new_suffix);

}

void qbinded_float_spinbox_t::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

void qbinded_float_spinbox_t::value_changed(double d) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = float(d);
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_combobox_t::load_value_ex() {

  if (m_binded_value) {
      setCurrentIndex(*m_binded_value);
    }

}

qbinded_combobox_t::qbinded_combobox_t(QWidget *parent) : QComboBox (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  connect(this,
          static_cast<void(qbinded_combobox_t::*)(int)>(&qbinded_combobox_t::currentIndexChanged),
          this,
          &qbinded_combobox_t::value_changed);

}

void qbinded_combobox_t::value_changed(int i) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = i;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_int3_input_t::load_value_ex() {

  if (m_binded_value) {
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
    }

}

void qbinded_int3_input_t::set_min_max_step(int min, int max, int step) {

  sb_x->setMinimum(min);
  sb_x->setMaximum(max);
  sb_x->setSingleStep(step);

  sb_y->setMinimum(min);
  sb_y->setMaximum(max);
  sb_y->setSingleStep(step);

  sb_z->setMinimum(min);
  sb_z->setMaximum(max);
  sb_z->setSingleStep(step);

}

qbinded_int3_input_t::qbinded_int3_input_t(QWidget *parent) : QWidget (parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);
  widget_layout->setContentsMargins(2, 2, 8, 2);

  auto make_spinbox = [](){
      auto ret = new QSpinBox;
      ret->setAlignment(Qt::AlignCenter);
      ret->setButtonSymbols(QAbstractSpinBox::NoButtons);
      return ret;
    };

  sb_x = make_spinbox();
  sb_y = make_spinbox();
  sb_z = make_spinbox();

  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addWidget(sb_z);

  connect(sb_x,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int3_input_t::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int3_input_t::spinbox_value_changed);

  connect(sb_z,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int3_input_t::spinbox_value_changed);

}

void qbinded_int3_input_t::spinbox_value_changed(int newval) {

  //we ignore newval here and make batch update based on inputs
  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_float3_input_t::load_value_ex() {

  if (m_binded_value) {
      m_ignore_state_change = true;
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
      m_ignore_state_change = false;
    }

}

void qbinded_float3_input_t::set_min_max_step(double min, double max, double step) {

  sb_x->setMinimum(min);
  sb_x->setMaximum(max);
  sb_x->setSingleStep(step);

  sb_y->setMinimum(min);
  sb_y->setMaximum(max);
  sb_y->setSingleStep(step);

  sb_z->setMinimum(min);
  sb_z->setMaximum(max);
  sb_z->setSingleStep(step);

}

void qbinded_float3_input_t::set_suffix(QString &new_suffix) {

  sb_x->setSuffix(new_suffix);
  sb_y->setSuffix(new_suffix);
  sb_z->setSuffix(new_suffix);

}

void qbinded_float3_input_t::set_empty_suffix() {

  sb_x->setSuffix("");
  sb_y->setSuffix("");
  sb_z->setSuffix("");

}

void qbinded_float3_input_t::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

qbinded_float3_input_t::qbinded_float3_input_t(QWidget *parent) : QWidget(parent) {

  widget_layout = new QHBoxLayout;
  widget_layout->setContentsMargins(2, 2, 8, 2);
  setLayout(widget_layout);

  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

  auto make_dspinbox = [](){
      auto ret = new QDoubleSpinBox;
      ret->setMaximumWidth(100);
      ret->setButtonSymbols(QAbstractSpinBox::NoButtons);
      ret->setAlignment(Qt::AlignCenter);
      ret->setLocale(QLocale::C);
      return ret;
    };

  sb_x = make_dspinbox();
  sb_y = make_dspinbox();
  sb_z = make_dspinbox();

  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addWidget(sb_z);

  widget_layout->addStretch(1);

  connect(sb_x,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input_t::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input_t::spinbox_value_changed);

  connect(sb_z,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input_t::spinbox_value_changed);

}

void qbinded_float3_input_t::spinbox_value_changed(double newval) {

  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_color3_input_t::load_value_ex() {

  app_state_t *astate = app_state_t::get_inst();

  QColor back_color = Qt::black;

  if (m_binded_value) {
      m_stored_color.setRgbF((*m_binded_value)[0],(*m_binded_value)[1], (*m_binded_value)[2]);
      back_color = m_stored_color;
    }

//  QPalette pal;
//  pal.setColor(QPalette::Background, back_color);
//  pal.setColor(QPalette::Foreground, Qt::white);
//  setPalette(pal);

//  setAutoFillBackground(true);

}

void qbinded_color3_input_t::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
      const QColor color = QColorDialog::getColor(m_stored_color, this, "Select Color");
      if (color.isValid() && m_binded_value) {
          (*m_binded_value)[0] = color.redF();
          (*m_binded_value)[1] = color.greenF();
          (*m_binded_value)[2] = color.blueF();
          load_value_ex();
          on_value_changed();
          app_state_t::get_inst()->make_viewport_dirty();
          update();
        }
    }

}

void qbinded_color3_input_t::paintEvent(QPaintEvent *event) {

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addRoundedRect(QRectF(1, 1, width() - 2, height() - 2), 1, 1);
  QPen pen(Qt::black, 1);
  p.setPen(pen);
  p.fillPath(path, m_stored_color);
  p.drawPath(path);

}

qbinded_color3_input_t::qbinded_color3_input_t(QWidget *parent) : QWidget (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setFixedWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setFixedHeight(astate->size_guide.obj_insp_ctrl_t2_h());

}

void qbinded_int_spinbox_t::load_value_ex() {

  if (m_binded_value) {
      setValue(*m_binded_value);
    }

}

void qbinded_int_spinbox_t::set_min_max_step(int new_min,
                                           int new_max,
                                           int new_step) {

  setRange(new_min, new_max);

}

qbinded_int_spinbox_t::qbinded_int_spinbox_t(QWidget *parent) : QSpinBox(parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setAlignment(Qt::AlignCenter);
  setButtonSymbols(QAbstractSpinBox::NoButtons);

  connect(this,
          static_cast<void (qbinded_int_spinbox_t::*)(int)>(&qbinded_int_spinbox_t::valueChanged),
          this,
          &qbinded_int_spinbox_t::value_changed);

}

void qbinded_int_spinbox_t::set_suffix(QString &new_suffix) {

  setSuffix(new_suffix);

}

void qbinded_int_spinbox_t::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

void qbinded_int_spinbox_t::value_changed(int value) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = value;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

qbinded_ws_item_combobox_t::qbinded_ws_item_combobox_t(QWidget *parent) {

  connect(this,
          static_cast<void (qbinded_ws_item_combobox_t::*)(int)>
          (&qbinded_ws_item_combobox_t::currentIndexChanged),
          this,
          &qbinded_ws_item_combobox_t::value_changed);

}

void qbinded_ws_item_combobox_t::bind_value(std::shared_ptr<ws_item_t> *_binded_value,
                                          ws_item_t *item_to_bind) {
  m_binded_ws_item_p = _binded_value;
  m_binded_ws_item = item_to_bind;
  m_binded_ws = item_to_bind->m_parent_ws;
  blockSignals(true);
  load_value();
  blockSignals(false);

}

void qbinded_ws_item_combobox_t::load_value() {

  app_state_t* astate = app_state_t::get_inst();

  rebuild_variants();

  if (!m_binded_ws_item_p->get()) {
//      astate->log("!m_binded_ws_item_p->get()");
      setCurrentIndex(0);
      return;
    }

  if (!m_binded_ws) {
//      astate->log("!m_binded_ws");
      setCurrentIndex(0);
      return;
    }

  for (size_t i = 0; i < count(); i++) {
      QString name_in_checkbox = itemText(i);
      std::string name_in_cb = name_in_checkbox.toStdString();
      if (m_binded_ws_item_p->get()->m_name == name_in_cb
          && m_binded_ws_item_p->get()->get_type() == m_type_id) {
//          astate->log(
//                fmt::format("qbinded_ws_item_combobox_t::load_value({}, {})", i, name_in_cb));
          setCurrentIndex(i); // 0 == None
          return;
        }
    }

}

void qbinded_ws_item_combobox_t::unbind_value() {

  blockSignals(true);

  m_binded_ws_item_p = nullptr;
  m_binded_ws_item = nullptr;
  m_binded_ws = nullptr;

}

void qbinded_ws_item_combobox_t::rebuild_variants() {

  blockSignals(true);
  clear();
  addItem(tr("None"));

  if (m_binded_ws) {
      for (auto &item : m_binded_ws->m_ws_items)
        if (item->get_type() == m_type_id)
          addItem(QString::fromStdString(item->m_name));

    }

  blockSignals(false);

}

void qbinded_ws_item_combobox_t::value_changed(int i) {

  if (!m_binded_ws) {
      setCurrentIndex(0);
      return;
    }

  if (i == 0) {
      *m_binded_ws_item_p = nullptr;
      if (m_binded_ws_item && m_updated_internally_event)
        m_binded_ws_item->updated_externally(m_upd_flag);
      return;
    }

  std::string name_to_find = currentText().toStdString();
  auto itm = m_binded_ws->get_by_name(name_to_find);

  if (itm && itm->get_type() == m_type_id) {
      *m_binded_ws_item_p = itm;
    }

  if (m_binded_ws_item && m_updated_internally_event)
    m_binded_ws_item->updated_externally(m_upd_flag);

}


qbinded_xgeom_color3_input_t::qbinded_xgeom_color3_input_t(QWidget *parent) : QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setMaximumHeight(astate->size_guide.obj_insp_ctrl_t2_h());
  setFrameStyle(QFrame::Panel);

}

void qbinded_xgeom_color3_input_t::bind_value(
    xgeometry<float, periodic_cell<float>> *_binded_xgeom,
    std::array<int, 3> _binding_indicies,
    size_t _binded_atom_id) {

  m_binded_xgeom = _binded_xgeom;
  m_binding_indicies = _binding_indicies;
  m_binded_atom_id = _binded_atom_id;

  load_value();

}

void qbinded_xgeom_color3_input_t::load_value() {

  if (m_binded_xgeom && m_binded_atom_id < m_binded_xgeom->nat()) {

      float _r = m_binded_xgeom->xfield<float>(m_binding_indicies[0], m_binded_atom_id);
      float _g = m_binded_xgeom->xfield<float>(m_binding_indicies[1], m_binded_atom_id);
      float _b = m_binded_xgeom->xfield<float>(m_binding_indicies[2], m_binded_atom_id);

      m_stored_color.setRgbF(_r, _g, _b);
      QPalette pal = palette();
      pal.setColor(QPalette::Background, m_stored_color);
      pal.setColor(QPalette::Foreground, Qt::white);
      setAutoFillBackground(true);
      setPalette(pal);
    }

}

void qbinded_xgeom_color3_input_t::unbind_value() {

  m_binded_xgeom = nullptr;

}

void qbinded_xgeom_color3_input_t::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
      const QColor color = QColorDialog::getColor(m_stored_color, this, "Select Color");
      if (color.isValid() && m_binded_xgeom && m_binded_atom_id < m_binded_xgeom->nat()) {

          float _r = color.redF();
          float _g = color.greenF();
          float _b = color.blueF();

          m_binded_xgeom->xfield<float>(m_binding_indicies[0], m_binded_atom_id) = _r;
          m_binded_xgeom->xfield<float>(m_binding_indicies[1], m_binded_atom_id) = _g;
          m_binded_xgeom->xfield<float>(m_binding_indicies[2], m_binded_atom_id) = _b;

          load_value();
          app_state_t::get_inst()->make_viewport_dirty();
        }
    }

}

void qbinded_xgeom_float_spinbox_t::bind_value(xgeometry<float, periodic_cell<float> > *_binded_xgeom,
                                             int _binding_index,
                                             size_t _binded_atom_id) {

  m_binded_xgeom = _binded_xgeom;
  m_binding_index = _binding_index;
  m_binded_atom_id = _binded_atom_id;

  load_value();

}

void qbinded_xgeom_float_spinbox_t::load_value() {

  if (m_binded_xgeom && m_binded_atom_id < m_binded_xgeom->nat()) {
      blockSignals(true);
      setValue(m_binded_xgeom->xfield<float>(m_binding_index, m_binded_atom_id));
      blockSignals(false);
    }

}

void qbinded_xgeom_float_spinbox_t::unbind_value() {
  m_binded_xgeom = nullptr;
}

void qbinded_xgeom_float_spinbox_t::set_min_max_step(double new_min,
                                                   double new_max,
                                                   double new_step,
                                                   int decimals) {

  setRange(new_min, new_max);
  setSingleStep(new_step);
  setDecimals(decimals);

}

qbinded_xgeom_float_spinbox_t::qbinded_xgeom_float_spinbox_t(QWidget *parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  connect(this,
          static_cast<void (qbinded_xgeom_float_spinbox_t::*)(double)>
          (&qbinded_xgeom_float_spinbox_t::valueChanged),
          this,
          &qbinded_xgeom_float_spinbox_t::value_changed);

}

void qbinded_xgeom_float_spinbox_t::set_suffix(QString &new_suffix) {

  setSuffix(new_suffix);

}

void qbinded_xgeom_float_spinbox_t::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

void qbinded_xgeom_float_spinbox_t::value_changed(double d) {

  if (m_binded_xgeom && m_binded_atom_id < m_binded_xgeom->nat()) {
      m_binded_xgeom->xfield<float>(m_binding_index, m_binded_atom_id) = float(d);
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_line_edit_t::load_value_ex() {

  if (m_binded_value) {
      setText(QString::fromStdString(*m_binded_value));
    }

}

qbinded_line_edit_t::qbinded_line_edit_t(QWidget *parent) : QLineEdit (parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  connect(this,
          &QLineEdit::editingFinished,
          this,
          &qbinded_line_edit_t::editing_finished);

}

void qbinded_line_edit_t::editing_finished() {

  app_state_t *astate = app_state_t::get_inst();

  if (m_binded_value) {
      *m_binded_value = text().toStdString();
      astate->make_viewport_dirty();
    }

}

void qbinded_int2b_input_t::load_value_ex() {

  if (m_binded_value) {
      sb_x->setValue((*m_binded_value)[0]);
      sb_x->setRange(0, (*m_binded_value)[2]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_y->setRange(0, (*m_binded_value)[2]);
    }

}

qbinded_int2b_input_t::qbinded_int2b_input_t(QWidget *parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);
  widget_layout->setContentsMargins(2, 2, 8, 2);

  auto make_spinbox = [](){
      auto ret = new QSpinBox;
      ret->setFixedWidth(40);
      ret->setAlignment(Qt::AlignCenter);
      ret->setButtonSymbols(QAbstractSpinBox::NoButtons);
      return ret;
    };

  sb_x = make_spinbox();
  sb_y = make_spinbox();

  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addStretch(0);

  connect(sb_x,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int2b_input_t::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int2b_input_t::spinbox_value_changed);

}

void qbinded_int2b_input_t::spinbox_value_changed(int newval) {

  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}
