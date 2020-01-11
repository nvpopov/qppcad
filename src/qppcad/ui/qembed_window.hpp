#ifndef QPPCAD_QEMBED_WINDOW
#define QPPCAD_QEMBED_WINDOW

#include <qppcad/core/qppcad.hpp>
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

        explicit qembed_window_t(QWidget *parent = nullptr);

    };

    class qembed_window_sub_header_t : public QFrame {

      Q_OBJECT

    public:

      QLabel *m_text;
      QVBoxLayout *m_main_lt;

      explicit qembed_window_sub_header_t(QWidget *parent = nullptr);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
