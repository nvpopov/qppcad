#ifndef QPPCAD_GEOM_VIEW_TYPE_SUMMARY_POPUP
#define QPPCAD_GEOM_VIEW_TYPE_SUMMARY_POPUP

#include <qppcad/qppcad.hpp>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class geom_view_type_summary_popup_t : public QDialog {

        Q_OBJECT

      public:

        geom_view_type_summary_popup_t(QWidget *parent);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
