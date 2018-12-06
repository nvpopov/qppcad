#ifndef QPP_CAD_PYTHON_CONSOLE_WIDGET_H
#define QPP_CAD_PYTHON_CONSOLE_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QTextEdit>
#include <QList>
#include <QDir>
#include <QVBoxLayout>
#include <QKeyEvent>

/*
 * inspired by avogadro (http://avogadro.cc/) pythonterminal extension
 */

namespace qpp {

  namespace cad {

    class python_text_editor_t;

    class python_console_widget_t : public QWidget {

        Q_OBJECT

      public:
        QVBoxLayout *console_layout;
        python_text_editor_t *py_tedit;
        python_console_widget_t(QWidget *parent);

    };

    class python_text_editor_t : public QTextEdit {

        Q_OBJECT

      public:
        QList<QString> m_commands;
        QString m_line_data;
        QFont m_font;

        int m_cur_cmd{0}, m_curs_pos{0}, m_indent{0};

        python_text_editor_t(QWidget *parent);
        void keyPressEvent(QKeyEvent *event);
        void last_command_reached();
        void wheelEvent (QWheelEvent * event);

      private slots:
        void run_cmd();
        void print_promt();
        void move_cursor_to_end();

    };

  }

}


#endif
