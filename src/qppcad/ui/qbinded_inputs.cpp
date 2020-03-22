#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/core/app_state.hpp>
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
      blockSignals(true);
      setValue(*m_binded_value);
      blockSignals(false);
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
        static_cast<void(qbinded_float_spinbox_t::*)(double)>(
          &qbinded_float_spinbox_t::valueChanged),
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

  //lineEdit()->setAlignment(Qt::AlignCenter);

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

void qbinded_int3_input_t::set_min_max_step(vector3<int> min,
                                            vector3<int> max,
                                            vector3<int> step) {

  sb_x->setMinimum(min[0]);
  sb_x->setMaximum(max[0]);
  sb_x->setSingleStep(step[0]);

  sb_y->setMinimum(min[1]);
  sb_y->setMaximum(max[1]);
  sb_y->setSingleStep(step[1]);

  sb_z->setMinimum(min[2]);
  sb_z->setMaximum(max[2]);
  sb_z->setSingleStep(step[2]);

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

void qbinded_float3_input_t::set_min_max_step_dec(double min, double max, double step,
                                                  std::optional<int> dec) {

  for (auto elem : {sb_x, sb_y, sb_z}) {

      elem->setMinimum(min);
      elem->setMaximum(max);
      elem->setSingleStep(step);
      if (dec) elem->setDecimals(*dec);

    }


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

void qbinded_float3_input_t::set_size(int new_size) {

  for (auto sb : {sb_x, sb_y, sb_z}) sb->setFixedWidth(new_size);

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

  QColor back_color = Qt::black;

  if (m_binded_value) {
      m_stored_color.setRgbF((*m_binded_value)[0],(*m_binded_value)[1], (*m_binded_value)[2]);
      back_color = m_stored_color;
    }

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
                                            iupdatable_externally_t *item_to_bind,
                                            workspace_t *binded_ws) {
  m_binded_ws_item_p = _binded_value;
  m_binded_item = item_to_bind;
  m_binded_ws = binded_ws;

  blockSignals(true);
  load_value();
  blockSignals(false);

}

void qbinded_ws_item_combobox_t::load_value() {

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
      std::string name_in_cb = name_in_checkbox.toStdString();

      if (m_binded_ws_item_p->get()->m_name == name_in_cb
          && m_binded_ws_item_p->get()->get_type() == m_type_id) {
          setCurrentIndex(i); // 0 == None
          return;
        }

    }

}

void qbinded_ws_item_combobox_t::unbind_value() {

  blockSignals(true);

  m_binded_ws_item_p = nullptr;
  m_binded_item = nullptr;
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
      if (m_binded_item && m_updated_externally_event)
        m_binded_item->updated_externally(m_upd_flag);
      return;
    }

  std::string name_to_find = currentText().toStdString();
  auto itm = m_binded_ws->get_by_name(name_to_find);

  if (itm && itm->get_type() == m_type_id) {
      *m_binded_ws_item_p = itm;
    }

  if (m_binded_item && m_updated_externally_event)
    m_binded_item->updated_externally(m_upd_flag);

}


qbinded_xgeom_color3_input_t::qbinded_xgeom_color3_input_t(QWidget *parent) : QFrame(parent) {

  app_state_t *astate = app_state_t::get_inst();
  setMaximumWidth(astate->size_guide.obj_insp_ctrl_max_w());
  setMaximumHeight(astate->size_guide.obj_insp_ctrl_t2_h());
  setFrameStyle(QFrame::Panel);

}

void qbinded_xgeom_color3_input_t::bind_value(
    xgeometry<float, periodic_cell<float> > *binded_xgeom,
    std::array<int, 3> binding_indicies,
    std::set<size_t> &&binded_atom_id) {

  m_binded_xgeom = binded_xgeom;
  m_binding_indicies = binding_indicies;
  m_binded_atom_id = binded_atom_id;

  load_value();

}

void qbinded_xgeom_color3_input_t::load_value() {

  if (m_binded_xgeom && !m_binded_atom_id.empty() &&
      *m_binded_atom_id.begin() < m_binded_xgeom->nat()) {

      size_t first_atom_id = *m_binded_atom_id.begin();

      float _r = m_binded_xgeom->xfield<float>(m_binding_indicies[0], first_atom_id);
      float _g = m_binded_xgeom->xfield<float>(m_binding_indicies[1], first_atom_id);
      float _b = m_binded_xgeom->xfield<float>(m_binding_indicies[2], first_atom_id);

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

      if (color.isValid() && m_binded_xgeom && !m_binded_atom_id.empty())
        for (const auto &atom_id : m_binded_atom_id)
          if (atom_id < m_binded_xgeom->nat()) {

              float _r = static_cast<float>(color.redF());
              float _g = static_cast<float>(color.greenF());
              float _b = static_cast<float>(color.blueF());

              m_binded_xgeom->xfield<float>(m_binding_indicies[0], atom_id) = _r;
              m_binded_xgeom->xfield<float>(m_binding_indicies[1], atom_id) = _g;
              m_binded_xgeom->xfield<float>(m_binding_indicies[2], atom_id) = _b;

              load_value();
            }

    }

  app_state_t::get_inst()->make_viewport_dirty();

}

void qbinded_xgeom_float_spinbox_t::bind_value(
    xgeometry<float, periodic_cell<float> > *binded_xgeom,
    int binding_index,
    std::set<size_t> &&binded_atom_id) {

  m_binded_xgeom = binded_xgeom;
  m_binding_index = binding_index;
  m_binded_atom_id = binded_atom_id;

  load_value();

}

void qbinded_xgeom_float_spinbox_t::load_value() {

  if (m_binded_xgeom && !m_binded_atom_id.empty() &&
      *m_binded_atom_id.begin() < m_binded_xgeom->nat()) {

      blockSignals(true);
      setValue(m_binded_xgeom->xfield<float>(m_binding_index, *m_binded_atom_id.begin()));
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
  setAlignment(Qt::AlignCenter);
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setLocale(QLocale::C);

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

  if (!m_binded_xgeom) return;

  for (const auto &atom_id : m_binded_atom_id)
    if (atom_id < m_binded_xgeom->nat()) {
        m_binded_xgeom->xfield<float>(m_binding_index, atom_id) = float(d);
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

void qbinded_int2b_input_t::set_min_max_step(int min, int max, int step) {

  sb_x->setRange(min, max);
  sb_x->setSingleStep(step);
  sb_y->setRange(min, max);
  sb_y->setSingleStep(step);
}

void qbinded_int2b_input_t::load_value_ex() {

  if (m_binded_value) {

      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);

      if ((*m_binded_value)[2] > 0) {
          sb_x->setRange(0, (*m_binded_value)[2]);
          sb_y->setRange(0, (*m_binded_value)[2]);
        }

    }

}

qbinded_int2b_input_t::qbinded_int2b_input_t(QWidget *parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);
  widget_layout->setContentsMargins(2, 2, 8, 2);

  auto make_spinbox = [](){
      auto ret = new QSpinBox;
      ret->setFixedWidth(70);
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

void qbinded_float2_input_t::load_value_ex() {

  if (m_binded_value) {
      sb_x->setValue((*m_binded_value)[0]);
      sb_y->setValue((*m_binded_value)[1]);
    }
}

void qbinded_float2_input_t::set_min_max_step(double min, double max, double step) {

  sb_x->setMinimum(min);
  sb_x->setMaximum(max);
  sb_x->setSingleStep(step);

  sb_y->setMinimum(min);
  sb_y->setMaximum(max);
  sb_y->setSingleStep(step);

}

void qbinded_float2_input_t::set_suffix(QString &new_suffix) {

  sb_x->setSuffix(new_suffix);
  sb_y->setSuffix(new_suffix);

}

void qbinded_float2_input_t::set_empty_suffix() {

  sb_x->setSuffix("");
  sb_y->setSuffix("");

}

void qbinded_float2_input_t::set_default_suffix() {

  app_state_t *astate = app_state_t::get_inst();
  set_suffix(astate->m_spatial_suffix);

}

qbinded_float2_input_t::qbinded_float2_input_t(QWidget *parent) {

  widget_layout = new QHBoxLayout;
  setLayout(widget_layout);
  widget_layout->setContentsMargins(2, 2, 8, 2);

  auto make_spinbox = [](){
      auto ret = new QDoubleSpinBox;
      ret->setFixedWidth(70);
      ret->setAlignment(Qt::AlignCenter);
      ret->setButtonSymbols(QAbstractSpinBox::NoButtons);
      ret->setLocale(QLocale::C);
      return ret;
    };

  sb_x = make_spinbox();
  sb_y = make_spinbox();

  widget_layout->addWidget(sb_x);
  widget_layout->addWidget(sb_y);
  widget_layout->addStretch(0);

  connect(sb_x,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float2_input_t::spinbox_value_changed);

  connect(sb_y,
          static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
          this,
          &qbinded_float2_input_t::spinbox_value_changed);

}

void qbinded_float2_input_t::spinbox_value_changed(double newval) {

  if (m_binded_value && !m_ignore_state_change) {
      (*m_binded_value)[0] = sb_x->value();
      (*m_binded_value)[1] = sb_y->value();
      on_value_changed();
      app_state_t::get_inst()->make_viewport_dirty();
    }

}

qbinded_bool_named_vector_t::qbinded_bool_named_vector_t(std::vector<QString> &&names,
                                                         QWidget *parent) {

  m_binded_names = names;
  m_binded_data.resize(m_binded_names.size(), nullptr);

  widget_layout = new QHBoxLayout;
  widget_layout->setContentsMargins(0, 1, 0, 0);
  setLayout(widget_layout);

  m_boxes.reserve(m_binded_names.size());
  m_labels.reserve(m_binded_names.size());

  for (size_t i = 0; i < m_binded_names.size(); i++) {

      QLabel *lbl = new QLabel(nullptr);
      lbl->setText(m_binded_names[i]);
      lbl->setProperty("s_class", "thin_label");
      QCheckBox *cbx = new QCheckBox(nullptr);
      widget_layout->addWidget(lbl);
      widget_layout->addWidget(cbx);
      m_boxes.push_back(cbx);
      m_labels.push_back(lbl);

      connect(cbx, &QCheckBox::stateChanged, [this, i](int state) {

          if (i < this->m_binded_data.size() && this->m_binded_data[i])
            *(this->m_binded_data[i]) = state != Qt::CheckState::Unchecked;

          app_state_t::get_inst()->make_viewport_dirty();

        });

    }

  widget_layout->addStretch(1);

}

void qbinded_bool_named_vector_t::bind_value(std::vector<bool *> &&binded_data) {

  if (m_binded_data.size() == m_binded_names.size() &&
      binded_data.size() == m_binded_names.size()) {

      m_binded_data = binded_data;

    } else {

      //raise error

    }

  load_value();

}

void qbinded_bool_named_vector_t::load_value() {

  if (m_binded_data.size() == m_binded_names.size())
    for (size_t i = 0; i < m_boxes.size(); i++)
      if (i < m_boxes.size() && m_boxes[i] && m_binded_data[i]) {

          m_boxes[i]->blockSignals(true);
          m_boxes[i]->setCheckState(
                i < m_binded_data.size() && m_binded_data[i] && *(m_binded_data[i]) ?
                  Qt::Checked :
                  Qt::Unchecked
                  );
          m_boxes[i]->blockSignals(false);

        }

}

void qbinded_bool_named_vector_t::unbind_value() {

  std::fill(std::begin(m_binded_data), std::end(m_binded_data), nullptr);

}

void qbinded_matrix3_input_t::load_value_ex() {

  for (size_t i = 0; i < 3; i++)
    for (size_t q = 0; q < 3; q++)
      m_sbx[q + i * 3]->setValue(m_binded_value ? (*m_binded_value)(i,q) : -1);

}

void qbinded_matrix3_input_t::set_min_max_step(double min, double max, double step) {

  for (auto sbx : m_sbx)
    if (sbx) {
        sbx->setMinimum(min);
        sbx->setMaximum(max);
        sbx->setSingleStep(step);
      }

}

void qbinded_matrix3_input_t::set_suffix(QString &new_suffix) {

  for (auto sbx : m_sbx)
    if (sbx) {
        sbx->setSuffix(new_suffix);
      }

}

void qbinded_matrix3_input_t::set_empty_suffix() {

  QString empty_suffix{""};
  set_suffix(empty_suffix);

}

void qbinded_matrix3_input_t::set_default_suffix() {

  set_empty_suffix();

}

qbinded_matrix3_input_t::qbinded_matrix3_input_t(QWidget *parent) : QWidget (parent) {

  m_main_lt = new QGridLayout;
  setLayout(m_main_lt);

  for (size_t i = 0; i < 3; i++)
    for (size_t q = 0; q < 3; q++) {


        QDoubleSpinBox *sb = new QDoubleSpinBox;
        //sb->setFixedWidth(60);
        sb->setAlignment(Qt::AlignCenter);
        sb->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sb->setLocale(QLocale::C);
        m_main_lt->addWidget(sb, i, q);
        m_sbx.push_back(sb);
        m_sbx_lookup.insert({sb, std::tuple{i,q}});
        sb->setMinimum(-100000000);
        sb->setMaximum(100000000);
        sb->setDecimals(4);
        sb->setSingleStep(0.001);
        connect(sb,
                static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this,
                &qbinded_matrix3_input_t::spinbox_value_changed);

      }

}

void qbinded_matrix3_input_t::spinbox_value_changed(double newval) {

  if (!m_binded_value) return;

  QObject* obj = sender();
  if (!obj) return;

  QDoubleSpinBox* sbx = qobject_cast<QDoubleSpinBox*>(obj);
  if (!sbx) return;

  auto it = m_sbx_lookup.find(sbx);
  if (it == m_sbx_lookup.end()) return;

  (*m_binded_value)(std::get<0>(it->second),std::get<1>(it->second)) = newval;

}

qbinded_float_named_vector_t::qbinded_float_named_vector_t(std::vector<QString> &&names,
                                                           QWidget *parent) {

  m_binded_names = names;
  m_binded_data.resize(m_binded_names.size(), nullptr);

  widget_layout = new QHBoxLayout;
  widget_layout->setContentsMargins(0, 1, 6, 0);
  setLayout(widget_layout);

  m_boxes.reserve(m_binded_names.size());
  m_labels.reserve(m_binded_names.size());

  for (size_t i = 0; i < m_binded_names.size(); i++) {

      QLabel *lbl = new QLabel(nullptr);
      lbl->setText(m_binded_names[i]);
      lbl->setProperty("s_class", "thin_label");
      QDoubleSpinBox *sbx = new QDoubleSpinBox(nullptr);
      sbx->setAlignment(Qt::AlignCenter);
      sbx->setButtonSymbols(QAbstractSpinBox::NoButtons);
      sbx->setLocale(QLocale::C);
      widget_layout->addWidget(lbl);
      widget_layout->addWidget(sbx);
      m_boxes.push_back(sbx);
      m_labels.push_back(lbl);

      connect(sbx,
              static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
              [this, i](double value) {

                if (i < this->m_binded_data.size() && this->m_binded_data[i]) {
                    *(this->m_binded_data[i]) = value;
                    if (m_binded_item
                        && i < this->m_updated_externally_event.size()
                        && this->m_updated_externally_event[i]
                        && i < this->m_upd_flag.size())
                      m_binded_item->updated_externally(this->m_upd_flag[i]);
                  }

                app_state_t::get_inst()->make_viewport_dirty();

              });

    }

  widget_layout->addStretch(1);

}

void qbinded_float_named_vector_t::set_min_max_step(float vmin, float vmax,
                                                    float vstep, std::optional<int> decs) {

  for (int i = 0; i < m_boxes.size(); i++)
    if (m_boxes[i]) {
        m_boxes[i]->setRange(vmin, vmax);
        m_boxes[i]->setSingleStep(vstep);
        if (decs) m_boxes[i]->setDecimals(*decs);
      }

}

void qbinded_float_named_vector_t::set_min_max_step_ex(std::vector<float> &&vmin,
                                                       std::vector<float> &&vmax,
                                                       std::vector<float> &&vstep,
                                                       std::vector<std::optional<int>> &&dec) {

  if (m_binded_data.size() == vmin.size()
      && m_binded_data.size() == vmax.size()
      && m_binded_data.size() == vstep.size()
      && m_binded_data.size() == dec.size()) {

      for (int i = 0; i < m_boxes.size(); i++)
        if (m_boxes[i]) {
            m_boxes[i]->setRange(vmin[i], vmax[i]);
            m_boxes[i]->setSingleStep(vstep[i]);
            if (dec[i]) m_boxes[i]->setDecimals(*dec[i]);
          }

    }

}

void qbinded_float_named_vector_t::bind_value(std::vector<float *> &&binded_data,
                                              iupdatable_externally_t *item_to_bind) {

  if (m_binded_data.size() == m_binded_names.size() &&
      binded_data.size() == m_binded_names.size()) {

      m_binded_data = binded_data;
      m_binded_item = item_to_bind;
      m_ignore_state_change = true;
      load_value();
      m_ignore_state_change = false;

    } else {

      //raise error

    }

  load_value();

}

void qbinded_float_named_vector_t::load_value() {

  if (m_binded_data.size() == m_binded_names.size())
    for (size_t i = 0; i < m_boxes.size(); i++)
      if (i < m_boxes.size() && m_boxes[i] && m_binded_data[i]) {

          m_boxes[i]->blockSignals(true);
          m_boxes[i]->setValue(*m_binded_data[i]);
          m_boxes[i]->blockSignals(false);

        }

}

void qbinded_float_named_vector_t::unbind_value() {

  std::fill(std::begin(m_binded_data), std::end(m_binded_data), nullptr);

}
