#include <qppcad/ui/qrich_error_message_box.hpp>
#include <QPushButton>
#include <QGroupBox>

using namespace qpp;
using namespace qpp::cad;

qrich_error_message_box_t::qrich_error_message_box_t(QString &message_title,
                                                     QString &error_mesage,
                                                     QString &error_detail) : QMessageBox () {

  setMinimumWidth(600);
  setWindowTitle(message_title);
  setIcon(QMessageBox::Critical);
  setText(error_mesage);
  setDetailedText(error_detail);

}
