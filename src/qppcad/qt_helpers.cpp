#include <qppcad/qt_helpers.hpp>
#include <QLabel>

using namespace qpp;
using namespace qpp::cad;

void qt_helpers::resize_form_lt_labels(QFormLayout *form_lt, int new_size) {
  for (int i = 0; i < form_lt->rowCount(); i++) {
      //get row lt
      QLayoutItem *lt = form_lt->itemAt(i, QFormLayout::LabelRole);
      if (lt) {
          lt->widget()->setMaximumWidth(new_size);
          lt->widget()->setMinimumWidth(new_size);
          QLabel *as_label = qobject_cast<QLabel *>(lt->widget());
          if (as_label) as_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }
}
