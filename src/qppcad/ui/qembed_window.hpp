#ifndef QPPCAD_QEMBED_WINDOW
#define QPPCAD_QEMBED_WINDOW

#include <qppcad/qppcad.hpp>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace qpp {

  namespace cad {

    class qembed_window_t : public QFrame {

        Q_OBJECT

      public:

        QLabel *ew_header;
        QVBoxLayout *main_lt;
        QVBoxLayout *main_lt_zero_lvl;

        QFrame *header_frm;
        QHBoxLayout *header_lt;

        explicit qembed_window_t(QWidget *parent = 0);

    };

  }

}

#endif
