#ifndef QPPCAD_QSPOILER_WIDGET
#define QPPCAD_QSPOILER_WIDGET

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    class qspoiler_widget_t : public QFrame {

        Q_OBJECT

      public:

        QVBoxLayout *m_main_lt;
        QFrame      *m_top_frm;
        QHBoxLayout *m_hbox_frm;
        QLabel      *m_lbl_frm;
        QPushButton *m_action_btn;
        QWidget     *m_widget_list;
        QVBoxLayout *m_widget_list_lt;

        bool m_state{true};
        bool m_collapsable{false};

        explicit qspoiler_widget_t(const QString & title = "",
                                   QWidget *parent = 0,
                                   bool _collapsable = true,
                                   int _spacing = 6,
                                   int _max_width = -1,
                                   bool header_add_spacing_at_the_end = true,
                                   int main_lt_spacing_at_the_end = 5);
        void set_top_border(bool border_enabled);
        void add_content_layout(QLayout *new_lt);
        void process_state();

      public slots:

        void collapse_button_pressed();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
