#include <qppcad/ui/qrealspinbox_delegate.hpp>

using namespace qpp;
using namespace qpp::cad;


qrealspinbox_delegate_t::qrealspinbox_delegate_t(QObject *parent) : QStyledItemDelegate(parent) {

}

void qrealspinbox_delegate_t::set_min_max_step(double _min, double _max, double _step) {
  m_spinbox_min = _min;
  m_spinbox_max = _max;
  m_spinbox_step = _step;
}

QWidget *qrealspinbox_delegate_t::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const {

  QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
  editor->setFrame(false);
  editor->setMinimum(m_spinbox_min);
  editor->setMaximum(m_spinbox_max);
  editor->setSingleStep(m_spinbox_step);
  //editor->setValue(index.data().toDouble());
  return editor;

}

void qrealspinbox_delegate_t::setEditorData(QWidget *editor, const QModelIndex &index) const {

  double value = index.data().toDouble();
  QDoubleSpinBox *qdspin_box = static_cast<QDoubleSpinBox*>(editor);
  qdspin_box->setValue(value);

}

void qrealspinbox_delegate_t::setModelData(QWidget *editor, QAbstractItemModel *model,
                                           const QModelIndex &index) const {

  QDoubleSpinBox *qdspin_box = static_cast<QDoubleSpinBox*>(editor);
  qdspin_box->interpretText();
  double value = qdspin_box->value();
  model->setData(index, value, Qt::EditRole);

}

void qrealspinbox_delegate_t::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem
                                                   &option, const QModelIndex &) const {

  editor->setGeometry(option.rect);

}
