#include <qppcad/ui/log_widget.hpp>
#include <qppcad/app_state.hpp>
#include <QScrollBar>

using namespace qpp;
using namespace qpp::cad;

log_widget_t::log_widget_t(QWidget *parent) : QTextEdit(parent) {

  app_state_t* astate = app_state_t::get_inst();

 // setEnabled(false);
  setReadOnly(true);
  setFocusPolicy(Qt::FocusPolicy::NoFocus);

  connect(astate->astate_evd,
          &app_state_event_disp_t::log_widget_query_signal,
          this,
          &log_widget_t::slot_log_widget_query);

}

void log_widget_t::slot_log_widget_query(const std::string &message) {

  append(QString::fromStdString(message));
  verticalScrollBar()->setValue(verticalScrollBar()->maximum());

  QString new_qss = QString("font-size:%1pt;").arg(17);
  setStyleSheet(new_qss);

}
