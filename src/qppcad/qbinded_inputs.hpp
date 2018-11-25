#ifndef QPP_CAD_QBINDED_INPUTS
#define QPP_CAD_QBINDED_INPUTS

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QCheckBox>

namespace qpp {

  namespace cad {

    class qbinded_checkbox : public QCheckBox {

        Q_OBJECT

      public:
        bool m_ignore_state_change{false};
        bool *m_binded_value{nullptr};
        void bind_value(bool *_binded_value);
        void load_value();
        void unbind_value();
        qbinded_checkbox(QWidget *parent = nullptr);
      public slots:
        void check_state_changed(int state);
    };

  }

}

#endif
