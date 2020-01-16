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

        QLabel *m_ew_header;
        QVBoxLayout *m_main_lt;
        QVBoxLayout *m_main_lt_zero_lvl;

        QFrame *m_header_frm;
        QHBoxLayout *m_header_lt;

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
