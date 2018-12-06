#ifndef QPP_CAD_PYTHON_CONSOLE_WIDGET_H
#define QPP_CAD_PYTHON_CONSOLE_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QTextEdit>
#include <QList>
#include <QDir>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

/*
 * inspired(==copied) by avogadro (http://avogadro.cc/) pythonterminal extension
 */

namespace qpp {

  namespace cad {

    class python_text_editor_syntax_highilighter_t;
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
        python_text_editor_syntax_highilighter_t *syntax_hl;

        int m_cur_cmd{0}, m_curs_pos{0}, m_indent{0};

        python_text_editor_t(QWidget *parent);
        void keyPressEvent(QKeyEvent *event);
        void last_command_reached();

      private slots:
        void run_cmd();
        void print_promt();
        void move_cursor_to_end();
        void clear_signal_received();

    };


    struct python_highlighting_rule_t{
        QRegExp pattern;
        QTextCharFormat format;
    };

    class python_text_editor_syntax_highilighter_t : public QSyntaxHighlighter {

        Q_OBJECT

      public:
        python_text_editor_syntax_highilighter_t(QTextDocument *parent = 0);

      protected:
        void highlightBlock(const QString &text);

      private:

        QVector<python_highlighting_rule_t> hl_rules;

        QRegExp comment_start_expression;
        QRegExp comment_end_expression;

        QTextCharFormat keyword_fmt;
        QTextCharFormat prompt_fmt;
        QTextCharFormat class_fmt;
        QTextCharFormat single_line_c_fmt;
        QTextCharFormat multi_line_c_fmt;
        QTextCharFormat quotation_fmt;
        QTextCharFormat function_fmt;
    };

  }

}


#endif
