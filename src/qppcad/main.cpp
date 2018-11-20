#include <iostream>
#include <QApplication>
#include <QStyleFactory>
#include <qppcad/main_window.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

int main (int argc, char **argv) {

  app_state_t::init_inst();
  app_state_t::get_inst()->init_managers();

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(8);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication app(argc, argv);
  app.setStyle(QStyleFactory::create("Fusion"));
  QPalette p;

  p = app.palette();
  p.setColor(QPalette::Window, QColor(55,55,55));
  p.setColor(QPalette::Button, QColor(53,53,53));
  p.setColor(QPalette::Highlight, QColor(142,45,197));
  p.setColor(QPalette::ButtonText, QColor(255,255,255));
  //p.setColor(QPalette::Text, QColor(200, 200, 200));
  app.setPalette(p);

  main_window w;
  w.showMaximized();
  return app.exec();

}
