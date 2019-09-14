#ifndef QPPCAD_PTABLE_RICH_WIDGET_H
#define QPPCAD_PTABLE_RICH_WIDGET_H

#include <qppcad/core/qppcad.hpp>
#include <QWidget>
#include <QDialog>

namespace qpp {

  namespace cad {

    class ptable_rich_widget_t : public QDialog {

        Q_OBJECT

      public:

        explicit ptable_rich_widget_t(QWidget *parent = nullptr);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
