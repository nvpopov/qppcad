#include <iostream>
#include <QApplication>
#include <qppcad/main_window.hpp>

using namespace qpp;
using namespace qpp::cad;

int main (int argc, char **argv) {

  QApplication app(argc, argv);
  main_window w;
  w.showMaximized();
  return app.exec();

}
