#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/app_state.hpp>
#include <QMouseEvent>
#include <QColorDialog>

using namespace qpp;
using namespace qpp::cad;

void qbinded_checkbox::load_value_ex() {

  if (m_binded_value) {
      if (*m_binded_value) setCheckState(Qt::Checked);
      else setCheckState(Qt::Unchecked);
    }

}

qbinded_checkbox::qbinded_checkbox(QWidget *parent) : QCheckBox (parent) {

  connect(this,
          static_cast<void(qbinded_checkbox::*)(int)>(&qbinded_checkbox::stateChanged),
          this,
          &qbinded_checkbox::check_state_changed);

}

void qbinded_checkbox::check_state_changed(int state) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = state == Qt::Checked;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_float_spinbox::load_value_ex() {

  if (m_binded_value) {
      setValue(*m_binded_value);
    }

}

void qbinded_float_spinbox::set_min_max_step(double new_min,
                                             double new_max,
                                             double new_step,
                                             int decimals) {
  setRange(new_min, new_max);
  setSingleStep(new_step);
  setDecimals(decimals);

}

qbinded_float_spinbox::qbinded_float_spinbox(QWidget *parent) : QDoubleSpinBox (parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  connect(this,
          static_cast<void(qbinded_float_spinbox::*)(double)>(&qbinded_float_spinbox::valueChanged),
          this,
          &qbinded_float_spinbox::value_changed);

}

void qbinded_float_spinbox::set_suffix(QString &new_suffix) {

  setSuffix(new_suffix);

}

void qbinded_float_spinbox::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

void qbinded_float_spinbox::value_changed(double d) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = float(d);
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_combobox::load_value_ex() {

  if (m_binded_value) {
      setCurrentIndex(*m_binded_value);
    }

}

qbinded_combobox::qbinded_combobox(QWidget *parent) : QComboBox (parent) {

  app_state_t *astate = app_state_t::get_inst();

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_combo_max_w());

  connect(this,
          static_cast<void(qbinded_combobox::*)(int)>(&qbinded_combobox::currentIndexChanged),
          this,
          &qbinded_combobox::value_changed);

}

void qbinded_combobox::value_changed(int i) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = i;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_int3_input::load_value_ex() {

  if (m_binded_value) {
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
    }

}

void qbinded_int3_input::set_min_max_step(int min, int max, int step) {

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

qbinded_int3_input::qbinded_int3_input(QWidget *parent) : QWidget (parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);
  widget_layout->setContentsMargins(2, 2, 8, 2);
  sb_x = new QSpinBox(this);
  sb_y = new QSpinBox(this);
  sb_z = new QSpinBox(this);
  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addWidget(sb_z);

  connect(sb_x,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int3_input::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int3_input::spinbox_value_changed);

  connect(sb_z,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this,
          &qbinded_int3_input::spinbox_value_changed);

}

void qbinded_int3_input::spinbox_value_changed(int newval) {

  //we ignore newval here and make batch update based on inputs
  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_float3_input::load_value_ex() {

  if (m_binded_value) {
      m_ignore_state_change = true;
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
      sb_z->setValue((*m_binded_value)[2]);
      m_ignore_state_change = false;
    }

}

void qbinded_float3_input::set_min_max_step(double min, double max, double step) {

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

void qbinded_float3_input::set_suffix(QString &new_suffix) {

  sb_x->setSuffix(new_suffix);
  sb_y->setSuffix(new_suffix);
  sb_z->setSuffix(new_suffix);

}

void qbinded_float3_input::set_empty_suffix() {

  sb_x->setSuffix("");
  sb_y->setSuffix("");
  sb_z->setSuffix("");

}

void qbinded_float3_input::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

qbinded_float3_input::qbinded_float3_input(QWidget *parent) : QWidget(parent) {

  widget_layout = new QHBoxLayout;
  widget_layout->setContentsMargins(2, 2, 8, 2);
  setLayout(widget_layout);

  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);

  sb_x = new QDoubleSpinBox(this);
  sb_x->setMaximumWidth(100);
  sb_x->setButtonSymbols(QAbstractSpinBox::NoButtons);

  sb_y = new QDoubleSpinBox(this);
  sb_y->setMaximumWidth(100);
  sb_y->setButtonSymbols(QAbstractSpinBox::NoButtons);

  sb_z = new QDoubleSpinBox(this);
  sb_z->setMaximumWidth(100);
  sb_z->setButtonSymbols(QAbstractSpinBox::NoButtons);

  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addWidget(sb_z);

  widget_layout->addStretch(1);

  connect(sb_x,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input::spinbox_value_changed);

  connect(sb_z,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float3_input::spinbox_value_changed);

}

void qbinded_float3_input::spinbox_value_changed(double newval) {

  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      (*m_binded_value)[2] = sb_z->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

void qbinded_color3_input::load_value_ex() {

  if (m_binded_value) {
      m_stored_color.setRgbF((*m_binded_value)[0],(*m_binded_value)[1], (*m_binded_value)[2]);
      QPalette pal = palette();
      pal.setColor(QPalette::Background, m_stored_color);
      pal.setColor(QPalette::Foreground, Qt::white);
      setAutoFillBackground(true);
      setPalette(pal);
    }

}

void qbinded_color3_input::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
      const QColor color = QColorDialog::getColor(m_stored_color, this, "Select Color");
      if (color.isValid() && m_binded_value) {
          (*m_binded_value)[0] = color.redF();
          (*m_binded_value)[1] = color.greenF();
          (*m_binded_value)[2] = color.blueF();
          load_value();
          on_value_changed();
          app_state_t::get_inst()->make_viewport_dirty();
        }
    }

}

qbinded_color3_input::qbinded_color3_input(QWidget *parent) : QFrame (parent) {

  app_state_t *astate = app_state_t::get_inst();
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setMaximumHeight(astate->size_guide.obj_insp_ctrl_t2_h());
  setFrameStyle(QFrame::Panel);
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::black);
  pal.setColor(QPalette::Foreground, Qt::white);
  setAutoFillBackground(true);
  setPalette(pal);

}

void qbinded_int_spinbox::load_value_ex() {

  if (m_binded_value) {
      setValue(*m_binded_value);
    }

}

void qbinded_int_spinbox::set_min_max_step(int new_min,
                                           int new_max,
                                           int new_step) {

  setRange(new_min, new_max);

}

qbinded_int_spinbox::qbinded_int_spinbox(QWidget *parent) : QSpinBox(parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  connect(this,
          static_cast<void (qbinded_int_spinbox::*)(int)>(&qbinded_int_spinbox::valueChanged),
          this,
          &qbinded_int_spinbox::value_changed);

}

void qbinded_int_spinbox::set_suffix(QString &new_suffix) {

  setSuffix(new_suffix);

}

void qbinded_int_spinbox::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

void qbinded_int_spinbox::value_changed(int value) {

  if (m_binded_value && !m_ignore_state_change) {
      *m_binded_value = value;
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

qbinded_ws_item_combobox::qbinded_ws_item_combobox(QWidget *parent) {

  connect(this,
          static_cast<void (qbinded_ws_item_combobox::*)(int)>
          (&qbinded_ws_item_combobox::currentIndexChanged),
          this,
          &qbinded_ws_item_combobox::value_changed);

}

void qbinded_ws_item_combobox::bind_value(std::shared_ptr<ws_item_t> *_binded_value,
                                          ws_item_t *item_to_bind) {
  m_binded_ws_item_p = _binded_value;
  m_binded_ws_item = item_to_bind;
  m_binded_ws = item_to_bind->m_parent_ws;
  load_value();

}

void qbinded_ws_item_combobox::load_value() {

  rebuild_variants();

  if (!m_binded_ws_item_p->get()) {
      setCurrentIndex(0);
      return;
    }

  if (!m_binded_ws) {
      setCurrentIndex(0);
      return;
    }

  for (size_t i = 0; i < count(); i++) {
      QString name_in_checkbox = itemText(i);
      auto itm = m_binded_ws->get_by_name(name_in_checkbox.toStdString());
      if (itm && itm->get_type() == m_type_id) {
          setCurrentIndex(i);
          return;
        }
    }

}

void qbinded_ws_item_combobox::unbind_value() {

  m_binded_ws_item_p = nullptr;
  m_binded_ws_item = nullptr;
  m_binded_ws = nullptr;

}

void qbinded_ws_item_combobox::rebuild_variants() {

  blockSignals(true);
  clear();
  addItem(tr("None"));

  if (m_binded_ws) {
      for (auto item : m_binded_ws->m_ws_items)
        if (item->get_type() == m_type_id) {
            addItem(QString::fromStdString(item->m_name));
          }
    }

  blockSignals(false);

}

void qbinded_ws_item_combobox::value_changed(int i) {

  if (!m_binded_ws) {
      setCurrentIndex(0);
      return;
    }

  if (i == 0) {
      *m_binded_ws_item_p = nullptr;
      return;
    }

  std::string name_to_find = currentText().toStdString();
  auto itm = m_binded_ws->get_by_name(name_to_find);

  if (itm && itm->get_type() == m_type_id) {
      *m_binded_ws_item_p = itm;
    }

}


qbinded_xgeom_color3_input::qbinded_xgeom_color3_input(QWidget *parent) : QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setMaximumHeight(astate->size_guide.obj_insp_ctrl_t2_h());
  setFrameStyle(QFrame::Panel);

}

void qbinded_xgeom_color3_input::bind_value(xgeometry<float, periodic_cell<float>> *_binded_xgeom,
                                            std::array<int, 3> _binding_indicies,
                                            size_t _binded_atom_id) {
  m_binded_xgeom = _binded_xgeom;
  m_binding_indicies = _binding_indicies;
  m_binded_atom_id = _binded_atom_id;

  load_value();

}

void qbinded_xgeom_color3_input::load_value() {

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

void qbinded_xgeom_color3_input::unbind_value() {

  m_binded_xgeom = nullptr;

}

void qbinded_xgeom_color3_input::mousePressEvent(QMouseEvent *event) {

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

void qbinded_xgeom_float_spinbox::bind_value(xgeometry<float, periodic_cell<float> > *_binded_xgeom,
                                             int _binding_index,
                                             size_t _binded_atom_id) {

  m_binded_xgeom = _binded_xgeom;
  m_binding_index = _binding_index;
  m_binded_atom_id = _binded_atom_id;

  load_value();

}

void qbinded_xgeom_float_spinbox::load_value() {

  if (m_binded_xgeom && m_binded_atom_id < m_binded_xgeom->nat()) {
      blockSignals(true);
      setValue(m_binded_xgeom->xfield<float>(m_binding_index, m_binded_atom_id));
      blockSignals(false);
    }

}

void qbinded_xgeom_float_spinbox::unbind_value() {
  m_binded_xgeom = nullptr;
}

void qbinded_xgeom_float_spinbox::set_min_max_step(double new_min,
                                                   double new_max,
                                                   double new_step,
                                                   int decimals) {

  setRange(new_min, new_max);
  setSingleStep(new_step);
  setDecimals(decimals);

}

qbinded_xgeom_float_spinbox::qbinded_xgeom_float_spinbox(QWidget *parent) {

  app_state_t *astate = app_state_t::get_inst();
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());

  connect(this,
          static_cast<void (qbinded_xgeom_float_spinbox::*)(double)>
          (&qbinded_xgeom_float_spinbox::valueChanged),
          this,
          &qbinded_xgeom_float_spinbox::value_changed);

}

void qbinded_xgeom_float_spinbox::set_suffix(QString &new_suffix) {

  setSuffix(new_suffix);

}

void qbinded_xgeom_float_spinbox::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

void qbinded_xgeom_float_spinbox::value_changed(double d) {

  if (m_binded_xgeom && m_binded_atom_id < m_binded_xgeom->nat()) {
      m_binded_xgeom->xfield<float>(m_binding_index, m_binded_atom_id) = float(d);
      app_state_t::get_inst()->make_viewport_dirty();
    }

}
