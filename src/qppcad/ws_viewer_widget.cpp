#include <qppcad/ws_viewer_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_viewer_widget_t::ws_viewer_widget_t(QWidget *parent) : QOpenGLWidget (parent) {

}

void ws_viewer_widget_t::initializeGL() {

  app_state_t* astate = app_state_t::get_inst();
  astate->init_glapi();
  astate->init_shaders();
  astate->init_meshes();
}

void ws_viewer_widget_t::resizeGL(int w, int h) {
  app_state_t* astate = app_state_t::get_inst();
  astate->viewport_size[0] = w;
  astate->viewport_size[1] = h;
  astate->viewport_size_c[0] = w;
  astate->viewport_size_c[1] = h;
}

void ws_viewer_widget_t::paintGL() {


  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

//  glapi->glClearColor(1.0, 1.0, 1.0, 1.0);
//  glapi->glClear(GL_COLOR_BUFFER_BIT);

  glViewport(static_cast<int>(0),
             static_cast<int>(0),
             static_cast<int>(astate->viewport_size_c(0)),
             static_cast<int>(astate->viewport_size_c(1)));

  if (astate->camera) astate->camera->update_camera();
  astate->ws_manager->render_current_workspace();

}
