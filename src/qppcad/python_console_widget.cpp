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
  m_font.setPointSize(16);
  setFont(m_font);
  print_promt();
}

void python_text_editor_t::keyPressEvent(QKeyEvent *event) {


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

void python_text_editor_t::wheelEvent(QWheelEvent *event) {
  event->accept();
}


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
      cursor.insertText(QLatin1String("... ") + indentString);
    }

  m_curs_pos = cursor.position();
  m_curs_pos -= m_indent;
  setTextCursor(cursor);

  //move_cursor_to_end();

}

void python_text_editor_t::move_cursor_to_end() {

  QTextCursor cursor(textCursor());
  if (cursor.position() < m_curs_pos)
    cursor.setPosition(m_curs_pos);
  setTextCursor(cursor);

}
