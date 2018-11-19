#include <qppcad/ws_viewer_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_viewer_widget_t::ws_viewer_widget_t(QWidget *parent) : QOpenGLWidget (parent) {

}

void ws_viewer_widget_t::initializeGL() {
  app_state_t* astate = app_state_t::get_inst();
  astate->init_shaders(context());
}

void ws_viewer_widget_t::resizeGL(int w, int h) {

}

void ws_viewer_widget_t::paintGL() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glClearColor(1.0, 1.0, 1.0, 1.0);
  f->glClear(GL_COLOR_BUFFER_BIT);
}
