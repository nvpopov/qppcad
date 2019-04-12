#ifndef QPPCAD_QRICH_ERROR_MESSAGE_BOX
#define QPPCAD_QRICH_ERROR_MESSAGE_BOX

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QImage>

namespace qpp {

  namespace cad {

    class qrich_error_message_box_t : public QMessageBox {

        Q_OBJECT

      public:

        qrich_error_message_box_t(QString &message_title,
                                  QString &error_mesage,
                                  QString &error_detail);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
