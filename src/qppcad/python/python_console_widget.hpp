#ifndef QPPCAD_PYTHON_CONSOLE_WIDGET_H
#define QPPCAD_PYTHON_CONSOLE_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QFrame>
#include <QTextEdit>
#include <QList>
#include <QDir>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QCompleter>
#include <QStringListModel>
#include <QStringList>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>

/*
 * inspired(==copied) by avogadro (http://avogadro.cc/) pythonterminal extension
 */

namespace qpp {

  namespace cad {

    class python_text_editor_syntax_highilighter_t;
    class python_text_editor_t;

    class python_console_widget_t : public QFrame {

        Q_OBJECT

      public:

        QHBoxLayout *console_lt;

        python_text_editor_t *py_tedit;
        QTextEdit *script_editor;
        QSplitter *edt_splitter;

        python_console_widget_t(QWidget *parent);

      public slots:

        void font_size_updated_signal_received();

    };

    class python_text_editor_t : public QTextEdit {

        Q_OBJECT

      public:

        QString m_line_data;
        QFont m_font;
        python_text_editor_syntax_highilighter_t *syntax_hl;
        QCompleter *m_c;
        QStringListModel *m_sl_model;
        QStringList m_wl;
        int m_cur_cmd{0}, m_curs_pos{0}, m_indent{0};

        python_text_editor_t(QWidget *parent);
        void keyPressEvent(QKeyEvent *event);
        void last_command_reached();
        QString text_under_cursor() const;

      public slots:

        void run_cmd();
        void print_promt(QString additional_text = "");
        void move_cursor_to_end();
        void clear_signal_received();
        void focus_signal_received();
        void insert_completion(const QString &completion);
        void insert_text_at_cursor(const QString &text_to_insert);

    };


    struct python_highlighting_rule_t{
        QRegExp pattern;
        QTextCharFormat format;
    };

    class python_text_editor_syntax_highilighter_t : public QSyntaxHighlighter {

        Q_OBJECT

      public:
        python_text_editor_syntax_highilighter_t(QTextDocument *parent = 0);
        QVector<python_highlighting_rule_t> hl_rules;

      protected:
        void highlightBlock(const QString &text);

      private:

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

  } // namespace qpp::cad

} // namespace qpp

#endif
