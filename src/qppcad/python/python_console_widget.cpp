#include <qppcad/python/python_console_widget.hpp>
#include <qppcad/app_state.hpp>

#include <QAction>

using namespace qpp;
using namespace qpp::cad;


python_console_widget_t::python_console_widget_t(QWidget *parent) : QFrame (parent) {

  app_state_t* astate = app_state_t::get_inst();

  py_tedit = new python_text_editor_t(nullptr);
  script_editor = new QTextEdit;
  script_editor_syntax_hl = new python_text_editor_syntax_highilighter_t(script_editor->document());

  console_lt = new QHBoxLayout;
  buttons_lt = new QVBoxLayout;

  btn_clear = new QPushButton();
  btn_clear->setIcon(QIcon("://images/outline-delete-24px.svg"));
  btn_clear->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                               astate->size_guide.tool_panel_icon_size()));
  btn_clear->setFixedSize(QSize(astate->size_guide.tool_panel_h(),
                                astate->size_guide.tool_panel_h()));
  btn_clear->setToolTip(tr("Clear the console output"));
  connect(btn_clear,
          &QPushButton::clicked,
          this,
          &python_console_widget_t::clear_btn_clicked);

  btn_editor_toggle = new QPushButton();
  btn_editor_toggle->setIcon(QIcon("://images/outline-receipt-24px.svg"));
  btn_editor_toggle->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                               astate->size_guide.tool_panel_icon_size()));
  btn_editor_toggle->setFixedSize(QSize(astate->size_guide.tool_panel_h(),
                                astate->size_guide.tool_panel_h()));
  btn_editor_toggle->setCheckable(true);
  btn_editor_toggle->setToolTip(tr("Toggle script editor"));
  connect(btn_editor_toggle,
          &QPushButton::toggled,
          this,
          &python_console_widget_t::editor_toggle_signal_toggled);

  btn_run_code = new QPushButton();
  btn_run_code->setIcon(QIcon("://images/outline-slideshow-24px.svg"));
  btn_run_code->setIconSize(QSize(astate->size_guide.tool_panel_icon_size(),
                               astate->size_guide.tool_panel_icon_size()));
  btn_run_code->setFixedSize(QSize(astate->size_guide.tool_panel_h(),
                                astate->size_guide.tool_panel_h()));
  btn_run_code->setToolTip(tr("Run script"));
  connect(btn_run_code,
          &QPushButton::clicked,
          this,
          &python_console_widget_t::run_script_button_clicked);

  buttons_lt->addWidget(btn_clear);
  buttons_lt->addWidget(btn_editor_toggle);
  buttons_lt->addWidget(btn_run_code);
  buttons_lt->addStretch(0);

  setLayout(console_lt);
  console_lt->addLayout(buttons_lt);

  edt_splitter = new QSplitter;
  edt_splitter->setHandleWidth(10);
  edt_splitter->addWidget(script_editor);
  edt_splitter->addWidget(py_tedit);
  edt_splitter->setCollapsible(0, false);
  edt_splitter->setCollapsible(1, false);

  console_lt->addWidget(edt_splitter);

  setMinimumHeight(200);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

  connect(astate->astate_evd,
          &app_state_event_disp_t::python_console_font_size_updated_signal,
          this,
          &python_console_widget_t::font_size_updated_signal_received);

  font_size_updated_signal_received();
  editor_toggle_signal_toggled(false);

}

void python_console_widget_t::editor_toggle_signal_toggled(bool checked) {

  script_editor->setVisible(checked);
  btn_run_code->setEnabled(checked);

}

void python_console_widget_t::font_size_updated_signal_received() {

  app_state_t* astate = app_state_t::get_inst();
  int new_font_point_size = astate->m_console_font_size;

  py_tedit->setStyleSheet(tr("font-size:%1pt;").arg(new_font_point_size));
  script_editor->setStyleSheet(tr("font-size:%1pt;").arg(new_font_point_size));

}

void python_console_widget_t::clear_btn_clicked() {

  py_tedit->clear();
  py_tedit->print_promt();

}

void python_console_widget_t::run_script_button_clicked() {

  app_state_t* astate = app_state_t::get_inst();
  QString _script = script_editor->toPlainText();
  if (_script.length() > 0) {
      astate->py_manager->execute(_script.toStdString());
      py_tedit->insert_text_at_cursor("execute_script()");
      py_tedit->append(QString::fromStdString(astate->py_manager->m_output_buffer));
      py_tedit->print_promt();
    }

}

python_text_editor_t::python_text_editor_t(QWidget *parent) : QTextEdit (parent) {

  syntax_hl = new python_text_editor_syntax_highilighter_t(document());
  print_promt();

  app_state_t* astate = app_state_t::get_inst();

  connect(astate->astate_evd,
          &app_state_event_disp_t::python_console_clear_requested_signal,
          this,
          &python_text_editor_t::clear_signal_received);

  connect(astate->astate_evd,
          &app_state_event_disp_t::python_console_focus_requested_signal,
          this,
          &python_text_editor_t::focus_signal_received);

  m_c = new QCompleter(this);
  m_c->popup()->setFont(m_font);
  m_c->popup()->setMinimumHeight(40);
  m_sl_model = new QStringListModel;
  m_wl.append("sel_it");
  m_wl.append("sel");
  m_wl.append("sel_ws");
  m_sl_model->setStringList(m_wl);

  m_c->setModel(m_sl_model);
  m_c->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
  m_c->setCaseSensitivity(Qt::CaseInsensitive);
  m_c->setWrapAround(false);

  m_c->setWidget(this);
  m_c->setCompletionMode(QCompleter::PopupCompletion);
  m_c->setCaseSensitivity(Qt::CaseInsensitive);

  connect(m_c,
          static_cast<void(QCompleter::*)(const QString &)>(&QCompleter::activated),
          this,
          &python_text_editor_t::insert_completion);

}

void python_text_editor_t::keyPressEvent(QKeyEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  bool is_completer_shortcut = false;

  if (m_c && m_c->popup()->isVisible()) {

      switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
          event->ignore();
          return;
        default:
          break;
        }
    }

  if (event->key() == Qt::Key_QuoteLeft || event->key() == Qt::Key_AsciiTilde) {
      parentWidget()->parentWidget()->setFocus();
      parentWidget()->parentWidget()->hide();
      return;
    }

  if (event->key() == Qt::Key_Escape) {

      parentWidget()->setFocus();
      return;
    }

  if (event->key() == Qt::Key_Control) {
      event->accept();
      return;
    }

  if (event->key() == Qt::Key_Up) {

      if (!astate->py_manager->m_commands.empty()) {
          m_cur_cmd--;
          if (m_cur_cmd < 0) m_cur_cmd = astate->py_manager->m_commands.size();
          last_command_reached();
        }

      event->accept();
      return;

    }

  if (event->key() == Qt::Key_Down) {

      if (!astate->py_manager->m_commands.empty()) {
          m_cur_cmd++;
          if(m_cur_cmd > astate->py_manager->m_commands.size()) m_cur_cmd = 0;
          last_command_reached();
        }

      event->accept();
      return;

    }

  if (event->key() == Qt::Key_Return) {
      QString text = toPlainText();
      QString t = text.right(text.size() - m_curs_pos);
      if (!t.isEmpty()) {
          astate->py_manager->m_commands.append(t);
          if (astate->py_manager->m_commands.size() > 100) {
              astate->py_manager->m_commands.removeFirst();
            }
        }
      m_cur_cmd = astate->py_manager->m_commands.size();
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

  if (event->key() == Qt::Key_Tab) {
      is_completer_shortcut = true;
    }

  if (!is_completer_shortcut) {
      move_cursor_to_end();
      QTextEdit::keyPressEvent(event);
    }

  if (m_c && m_c->popup()->isVisible()) {

      QString completion_prefix = text_under_cursor();
      if (completion_prefix != m_c->completionPrefix()) {
          app_state_t* astate = app_state_t::get_inst();
          astate->py_manager->get_completion_list(completion_prefix, m_wl);
          m_sl_model->setStringList(m_wl);
          m_c->setCompletionPrefix(completion_prefix);
        }

      if (m_wl.size() == 0)
        m_c->popup()->hide();
    }

  if (is_completer_shortcut) {
      QString completion_prefix = text_under_cursor();
      app_state_t* astate = app_state_t::get_inst();
      astate->py_manager->get_completion_list(completion_prefix, m_wl);
      m_sl_model->setStringList(m_wl);

      if (completion_prefix != m_c->completionPrefix()) {
          m_c->setCompletionPrefix(completion_prefix);
          m_c->popup()->setStyleSheet("font-size:16pt;");
          m_c->popup()->setCurrentIndex(m_c->completionModel()->index(0, 0));
        }

      QRect cr = cursorRect();
      cr.setWidth(m_c->popup()->sizeHintForColumn(0)
                  + m_c->popup()->verticalScrollBar()->sizeHint().width());
      m_c->complete(cr);
    }

}

void python_text_editor_t::last_command_reached() {

  app_state_t *astate = app_state_t::get_inst();

  if (m_cur_cmd == astate->py_manager->m_commands.size()) {
      setText(toPlainText().left(m_curs_pos));
      QTextCursor cursor(textCursor());
      cursor.movePosition(QTextCursor::End);
      setTextCursor(cursor);
    } else {
      setText(toPlainText().left(m_curs_pos));
      QTextCursor cursor(textCursor());
      cursor.movePosition(QTextCursor::End);
      cursor.insertText(astate->py_manager->m_commands.at(m_cur_cmd));
      cursor.movePosition(QTextCursor::End);
      setTextCursor(cursor);
    }

}

QString python_text_editor_t::text_under_cursor() const {

  QString text = toPlainText();
  text = text.right(text.size() - m_curs_pos);

  int last = text.lastIndexOf(QChar::Space);
  std::cout <<"LAST " << last << " TEXT " << text.toStdString() << std::endl;

  int last_bracket = text.lastIndexOf("(");
  int last_eq_sign = text.lastIndexOf("=");

  if (last == -1 && last_bracket != -1) last = last_bracket + 1;
  if (last_eq_sign != -1 && last_eq_sign > last) last = last_eq_sign + 1;

  if (last == -1) {
      return text;
    } else {
      std::cout <<"LAST TEXT " << text.right(text.size()-last).toStdString() << std::endl;
      return text.right(text.size()-last).trimmed();
    }

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
              QString proc_command = text.replace(".  .  .", "    ");
              bool result = astate->py_manager->execute(proc_command.toStdString());
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

void python_text_editor_t::print_promt(QString additional_text) {

  QTextCursor cursor(textCursor());
  cursor.clearSelection();
  cursor.movePosition(QTextCursor::End);

  if (m_line_data.isEmpty()) {
      cursor.insertText(QLatin1String(">>> "));
    } else  {
      QString indentString;
      for (int i = 0; i < m_indent; i++)
        indentString += QLatin1String(" ");
      cursor.insertText(QLatin1String(".  .  . ") + indentString + additional_text);
    }

  m_curs_pos = cursor.position();
  m_curs_pos -= m_indent;
  setTextCursor(cursor);

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

void python_text_editor_t::focus_signal_received() {

  setFocus();

}

void python_text_editor_t::insert_completion(const QString &completion) {

  if (m_c->widget() != this) return;
  QTextCursor tc = textCursor();
  int extra = completion.length() - m_c->completionPrefix().length();
  tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::EndOfWord);
  tc.insertText(completion.right(extra));
  setTextCursor(tc);

}

void python_text_editor_t::insert_text_at_cursor(const QString &text_to_insert) {

  if (m_c->widget() != this) return;
  QTextCursor tc = textCursor();
  int extra = text_to_insert.length();
  tc.movePosition(QTextCursor::Left);
  tc.movePosition(QTextCursor::EndOfWord);
  tc.insertText(text_to_insert.right(extra));
  setTextCursor(tc);

}

python_text_editor_syntax_highilighter_t::python_text_editor_syntax_highilighter_t(
    QTextDocument *parent) : QSyntaxHighlighter (parent) {

  python_highlighting_rule_t rule;

  prompt_fmt.setForeground(Qt::green);

  //  prompt_fmt.setFontWeight(QFont::Bold);
  QStringList promptPatterns;
  promptPatterns << ">>>" << "\\.\\.\\.";

  foreach (const QString &pattern, promptPatterns) {
      rule.pattern = QRegExp(pattern);
      rule.format = prompt_fmt;
      hl_rules.append(rule);
    }

  keyword_fmt.setForeground(Qt::lightGray);
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

  //class_fmt.setFontWeight(QFont::Bold);
  class_fmt.setForeground(Qt::lightGray);
  rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
  rule.format = class_fmt;
  hl_rules.append(rule);

  single_line_c_fmt.setForeground(Qt::red);
  rule.pattern = QRegExp("#[^\n]*");
  rule.format = single_line_c_fmt;
  hl_rules.append(rule);

  multi_line_c_fmt.setForeground(Qt::red);

  quotation_fmt.setForeground(Qt::lightGray);
  rule.pattern = QRegExp("\".*\"");
  rule.format = quotation_fmt;
  hl_rules.append(rule);

  //function_fmt.setFontWeight(QFont::Bold);
  function_fmt.setForeground(Qt::lightGray);
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
