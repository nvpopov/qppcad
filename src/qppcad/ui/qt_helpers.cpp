#include <qppcad/ui/qt_helpers.hpp>
#include <iostream>
#include <QLabel>

using namespace qpp;
using namespace qpp::cad;

void qt_helpers::resize_form_lt_lbl(QLabel *label, int new_size) {

  label->setMaximumWidth(new_size);
  label->setMinimumWidth(new_size);
  label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
  label->setText(label->text() + " : ");

}

void qt_helpers::resize_form_lt_lbls(QFormLayout *form_lt, int new_size) {
  for (int i = 0; i < form_lt->rowCount(); i++) {
      //get row lt
      QLayoutItem *lt = form_lt->itemAt(i, QFormLayout::LabelRole);
      if (lt) {
          QLabel *as_label = qobject_cast<QLabel *>(lt->widget());
          if (as_label) resize_form_lt_lbl(as_label, new_size);
        }
    }
}

void qt_helpers::form_lt_hide_row(QFormLayout *form_lt,
                                  QLabel *field_label,
                                  QWidget *field_widget) {

  //TODO: throw error
  if (!form_lt || !field_label || !field_widget) {
      return;
    }

  int row_ptr;
  QFormLayout::ItemRole item_role_ptr;
  form_lt->getWidgetPosition(field_widget, &row_ptr, &item_role_ptr);

  //the widget is already hidden
  if (row_ptr == -1) {
      //std::cout << "::form_lt_hide_row row_ptr == -1" << std::endl;
      return;
    }

  field_label->hide();
  field_widget->hide();

  form_lt->takeRow(row_ptr);

}

void qt_helpers::form_lt_insert_before(QFormLayout *form_lt,
                                       QWidget *target,
                                       QLabel *field_label,
                                       QWidget *field_widget) {

  if (!form_lt || !target || !field_label || !field_widget) {
      return;
    }

  int row_ptr_self;
  QFormLayout::ItemRole item_role_ptr_self;
  form_lt->getWidgetPosition(field_widget, &row_ptr_self, &item_role_ptr_self);

  //the widget is already shown
  if (row_ptr_self != -1) {
      //std::cout << "::form_lt_insert_before row_ptr_self != -1" << std::endl;
      return;
    }

  int row_ptr;

  if (target) {
      QFormLayout::ItemRole item_role_ptr;
      form_lt->getWidgetPosition(target, &row_ptr, &item_role_ptr);

      if (row_ptr == -1) {
          //std::cout << "::form_lt_insert_before rrow_ptr == -1" << std::endl;
          row_ptr = form_lt->rowCount() - 1;
        }
    } else {
      row_ptr = 0;
    }

  field_label->show();
  field_widget->show();
  form_lt->insertRow(row_ptr + 1, field_label, field_widget);

}

void qt_helpers::form_lt_ctrl_visibility(bool show,
                                         QFormLayout *form_lt,
                                         QWidget *target,
                                         QLabel *field_label,
                                         QWidget *field_widget) {

  if (show) form_lt_insert_before(form_lt, target, field_label, field_widget);
  else form_lt_hide_row(form_lt, field_label, field_widget);

}
