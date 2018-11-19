#include <iostream>
#include <QApplication>
#include <qppcad/main_window.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

int main (int argc, char **argv) {

  app_state_t::init_inst();

  QApplication app(argc, argv);
  main_window w;
  w.showMaximized();
  return app.exec();

}
