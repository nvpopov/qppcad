#ifndef QPP_CAD_AXIAL_SCALE_WIDGET
#define QPP_CAD_AXIAL_SCALE_WIDGET

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

namespace qpp {

  namespace cad {

    class axial_scale_widget_t : public QDialog {

        Q_OBJECT

      public:
        axial_scale_widget_t();

    };

  }

}


#endif
