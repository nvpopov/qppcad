#include <qppcad/python_console_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


python_console_widget_t::python_console_widget_t(QWidget *parent) : QWidget (parent) {

  py_tedit = new python_text_editor_t(nullptr);
  console_layout = new QVBoxLayout;
  setLayout(console_layout);
  console_layout->addWidget(py_tedit);

}

python_text_editor_t::python_text_editor_t(QWidget *parent) : QTextEdit (parent) {
  //m_commands.append("");
  syntax_hl = new python_text_editor_syntax_highilighter_t(document());
  m_font.setPointSize(14);
  setFont(m_font);
  print_promt();

  app_state_t* astate = app_state_t::get_inst();

  connect(astate->astate_evd, &app_state_event_disp_t::python_console_clear_requested_signal,
          this, &python_text_editor_t::clear_signal_received);
}

void python_text_editor_t::keyPressEvent(QKeyEvent *event) {

  if (event->key() == Qt::Key_QuoteLeft || event->key() == Qt::Key_AsciiTilde) {
      return;
    }

  if (event->key() == Qt::Key_Escape) {

     parentWidget()->setFocus();
     return;
    }

  if (event->key() == Qt::Key_Tab) {
      return;
    }

  if (event->key() == Qt::Key_Control) {
      event->accept();
      return;
    }

  if (event->modifiers() == Qt::ControlModifier) {
      event->accept();
      return;
    }

  if (event->key() == Qt::Key_Up) {

      if (!m_commands.empty()) {
          m_cur_cmd--;
          if (m_cur_cmd < 0) m_cur_cmd = m_commands.size();
          last_command_reached();
        }

      event->accept();
      return;

    }

  if (event->key() == Qt::Key_Down) {

      if (!m_commands.empty()) {
          m_cur_cmd++;
          if(m_cur_cmd > m_commands.size()) m_cur_cmd = 0;
          last_command_reached();
        }

      event->accept();
      return;

    }

  if (event->key() == Qt::Key_Return) {

      QString text = toPlainText();

      QString t = text.right(text.size() - m_curs_pos);

      if (!t.isEmpty()) {
          m_commands.append(t);

          if (m_commands.size() > 100) {
              m_commands.removeFirst();
            }
        }

      m_cur_cmd = m_commands.size();
      run_cmd();
      event->accept();
      return;
    }

  if (event->key() == Qt::Key_Backspace) {

      QTextCursor cursor(textCursor());
      if (cursor.position() <= m_curs_pos) {
          event->accept();
          return;
        }

    }

  if (event->key() == Qt::Key_Home) {

      QTextCursor cursor(textCursor());
      cursor.setPosition(m_curs_pos);
      setTextCursor(cursor);
      event->accept();
      return;

    }

  move_cursor_to_end();

  QTextEdit::keyPressEvent(event);
}

void python_text_editor_t::last_command_reached() {

  if (m_cur_cmd == m_commands.size()) {
      setText(toPlainText().left(m_curs_pos));
      QTextCursor cursor(textCursor());
      cursor.movePosition(QTextCursor::End);
      setTextCursor(cursor);
    } else {
      setText(toPlainText().left(m_curs_pos));
      QTextCursor cursor(textCursor());
      cursor.movePosition(QTextCursor::End);
      cursor.insertText(m_commands.at(m_cur_cmd));
      cursor.movePosition(QTextCursor::End);
      setTextCursor(cursor);
    }

}

//void python_text_editor_t::wheelEvent(QWheelEvent *event) {
//  event->accept();
//}


void python_text_editor_t::run_cmd() {

  int indent = 0;
  QString text = toPlainText();
  text = text.right(text.size() - m_curs_pos);

  QString indentString;

  if (!text.trimmed().isEmpty()) {
      QString line = text;
      while (line.startsWith(' ')) {
          line.remove(0, 1);
          indent++;
        }
      line += text.trimmed();

      if (line.endsWith(':')) {
          indent += 2;
          m_line_data.append(text + "\n");
          append(QLatin1String(""));
        } else {
          if (indent && !m_line_data.isEmpty()) {
              m_line_data.append(text + "\n");
              append(QLatin1String(""));
            } else {
              app_state_t* astate = app_state_t::get_inst();
              bool result = astate->py_manager->execute(text.toStdString());
              append(QString::fromStdString(astate->py_manager->m_output_buffer));
            }
        }

      for (int i = 0; i < indent; ++i)
        indentString += QLatin1String(" ");

    } else {
      app_state_t* astate = app_state_t::get_inst();
      bool result = astate->py_manager->execute(m_line_data.toStdString());
      append(QString::fromStdString(astate->py_manager->m_output_buffer));
      m_line_data.clear();
    }

  m_indent = indent;
  print_promt();

}

void python_text_editor_t::print_promt() {

  QTextCursor cursor(textCursor());
  cursor.clearSelection();
  cursor.movePosition(QTextCursor::End);

  if (m_line_data.isEmpty()) {
      cursor.insertText(QLatin1String(">>> "));
    } else  {
      QString indentString;
      for (int i = 0; i < m_indent; i++)
        indentString += QLatin1String(" ");
      cursor.insertText(QLatin1String(".  .  . ") + indentString);
    }

  m_curs_pos = cursor.position();
  m_curs_pos -= m_indent;
  setTextCursor(cursor);

  //move_cursor_to_end();

}

void python_text_editor_t::move_cursor_to_end() {

  QTextCursor cursor(textCursor());
  if (cursor.position() < m_curs_pos) cursor.setPosition(m_curs_pos);
  setTextCursor(cursor);

}

void python_text_editor_t::clear_signal_received() {
  clear();
  //print_promt();
}

python_text_editor_syntax_highilighter_t::python_text_editor_syntax_highilighter_t(
    QTextDocument *parent) : QSyntaxHighlighter (parent) {

  python_highlighting_rule_t rule;

  prompt_fmt.setForeground(Qt::red);
//  prompt_fmt.setFontWeight(QFont::Bold);
  QStringList promptPatterns;
  promptPatterns << ">>>" << "\\.\\.\\.";

  foreach (const QString &pattern, promptPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = prompt_fmt;
      hl_rules.append(rule);
    }

  keyword_fmt.setForeground(Qt::darkGreen);
  //keyword_fmt.setFontWeight(QFont::Bold);
  QStringList keywordPatterns;

  // Python keywords
  keywordPatterns << "\\band\\b" << "\\bdel\\b" << "\\bfrom\\b"
                  << "\\bnot\\b" << "\\bwhile\\b" << "\\bas\\b"
                  << "\\belif\\b" << "\\bglobal\\b" << "\\bor\\b"
                  << "\\bwith\\b" << "\\bassert\\b" << "\\belse\\b"
                  << "\\bif\\b" << "\\bpass\\b" << "\\byield\\b"
                  << "\\bbreak\\b" << "\\bexcept\\b" << "\\bimport\\b"
                  << "\\bprint\\b" << "\\bclass\\b" << "\\bexec\\b"
                  << "\\bin\\b" << "\\braise\\b" << "\\bcontinue\\b"
                  << "\\bfinally\\b" << "\\bis\\b" << "\\breturn\\b"
                  << "\\bdef\\b" << "\\bfor\\b" << "\\blambda\\b"
                  << "\\btry\\b";

  foreach (const QString &pattern, keywordPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = keyword_fmt;
      hl_rules.append(rule);
    }

  class_fmt.setFontWeight(QFont::Bold);
  class_fmt.setForeground(Qt::darkGreen);
  rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
  rule.format = class_fmt;
  hl_rules.append(rule);

  single_line_c_fmt.setForeground(Qt::red);
  rule.pattern = QRegExp("#[^\n]*");
  rule.format = single_line_c_fmt;
  hl_rules.append(rule);

  multi_line_c_fmt.setForeground(Qt::red);

  quotation_fmt.setForeground(Qt::darkGreen);
  rule.pattern = QRegExp("\".*\"");
  rule.format = quotation_fmt;
  hl_rules.append(rule);

  function_fmt.setFontWeight(QFont::Bold);
  function_fmt.setForeground(Qt::darkGreen);
  rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
  rule.format = function_fmt;
  hl_rules.append(rule);

  comment_start_expression = QRegExp("/\\*");
  comment_end_expression = QRegExp("\\*/");
}

void python_text_editor_syntax_highilighter_t::highlightBlock(const QString &text) {

  foreach (const python_highlighting_rule_t &rule, hl_rules) {
      QRegExp expression(rule.pattern);
      int index = text.indexOf(expression);
      while (index >= 0) {
          int length = expression.matchedLength();
          setFormat(index, length, rule.format);
          index = text.indexOf(expression, index + length);
        }
    }

  setCurrentBlockState(0);

  int startIndex = 0;
  if (previousBlockState() != 1)
    startIndex = text.indexOf(comment_start_expression);

  while (startIndex >= 0) {
      int endIndex = text.indexOf(comment_end_expression, startIndex);
      int commentLength;
      if (endIndex == -1) {
          setCurrentBlockState(1);
          commentLength = text.length() - startIndex;
        } else {
          commentLength = endIndex - startIndex + comment_end_expression.matchedLength();
        }
      setFormat(startIndex, commentLength, multi_line_c_fmt);
      startIndex = text.indexOf(comment_start_expression, startIndex + commentLength);
    }
}
