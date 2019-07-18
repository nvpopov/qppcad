#ifndef QPPCAD_LOG_WIDGET_H
#define QPPCAD_LOG_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QTextEdit>

namespace qpp {

  namespace cad {

    class log_widget_t : public QTextEdit {

        Q_OBJECT

      public:

        explicit log_widget_t(QWidget *parent = 0);

      public slots:

       void slot_log_widget_query(const std::string &message);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
