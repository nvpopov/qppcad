#include <qppcad/ws_viewer_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_viewer_widget_t::ws_viewer_widget_t(QWidget *parent) : QOpenGLWidget (parent) {
  m_update_timer = new QTimer;
  //m_update_timer->setInterval(16);
  m_update_timer->connect(m_update_timer, &QTimer::timeout, this,
                          &ws_viewer_widget_t::update_cycle);
  m_update_timer->start(16);
  setMouseTracking(true);
  setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
  connect(this, &ws_viewer_widget_t::customContextMenuRequested,
          this, &ws_viewer_widget_t::show_context_menu);
}

void ws_viewer_widget_t::update_cycle() {

  app_state_t* astate = app_state_t::get_inst();


  //if (!astate->is_mouse_moving) astate->mouse_distance_pp = 0;

  if (astate->ws_manager->has_wss()) {
      auto cur_ws = astate->ws_manager->get_current();
      cur_ws->update(0.016);
    }

  if (astate->is_viewport_dirty()) {

      astate->cleanup_viewport();

      if (astate->camera && (astate->camera->m_rotate_camera || astate->camera->m_move_camera)) {
          astate->camera->update_camera();
          astate->camera->m_rotate_camera = false;
          astate->camera->m_move_camera = false;
        }

      repaint();
      //if (astate->camera)
    }

  astate->is_mouse_moving = false;

}

void ws_viewer_widget_t::initializeGL() {

  app_state_t* astate = app_state_t::get_inst();

  astate->init_glapi();
  astate->init_shaders();
  astate->init_meshes();

  glapi_t* glapi = astate->glapi;

}

void ws_viewer_widget_t::resizeGL(int w, int h) {
  app_state_t* astate = app_state_t::get_inst();
  astate->viewport_size[0] = w;
  astate->viewport_size[1] = h;
  astate->viewport_size_c[0] = w;
  astate->viewport_size_c[1] = h;
  if (astate->camera) astate->camera->update_camera();
}

void ws_viewer_widget_t::paintGL() {


  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

  //  glapi->glClearColor(1.0, 1.0, 1.0, 1.0);
  //  glapi->glClear(GL_COLOR_BUFFER_BIT);

  glapi->glViewport(static_cast<int>(0),
                    static_cast<int>(0),
                    static_cast<int>(astate->viewport_size_c(0)),
                    static_cast<int>(astate->viewport_size_c(1)));

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.beginNativePainting();

  glapi->glEnable(GL_DEPTH_TEST);
  glapi->glDepthFunc(GL_LEQUAL);
  glapi->glEnable(GL_CULL_FACE);
  glapi->glCullFace(GL_BACK);
  astate->ws_manager->render_current_workspace();

  glapi->glDisable(GL_DEPTH_TEST);

  painter.endNativePainting();

  astate->ws_manager->render_current_workspace_overlay(&painter);
  painter.end();

  //std::cout<<"REPAINT"<<std::endl;

}

void ws_viewer_widget_t::mousePressEvent(QMouseEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  if (event) {

      if (event->button() == Qt::LeftButton)  {
          astate->mouse_lb_pressed = true;
          astate->ws_manager->mouse_click();
          astate->mouse_distance_pp = 0.0f;
        }

      if (event->button() == Qt::RightButton) {
          astate->mouse_rb_pressed = true;

        }

      if (event->button() == Qt::MiddleButton) astate->mouse_md_pressed = true;
    }
  //fmt::print(std::cout, "mousePressEvent\n");
}

void ws_viewer_widget_t::mouseReleaseEvent(QMouseEvent *event) {

  app_state_t* astate = app_state_t::get_inst();
  //fmt::print(std::cout, "mouseReleaseEvent\n");
  if (event) {

      bool need_to_cancel_cam_transform = false;

      if (event->button() == Qt::LeftButton) {
          astate->mouse_lb_pressed = false;
          //          if (astate->camera && !astate->camera->m_rotate_camera && !astate->camera->m_move_camera
          //              && astate->mouse_distance_pp < 10)

          need_to_cancel_cam_transform = true;
        }

      if (event->button() == Qt::RightButton) {
          astate->mouse_rb_pressed = false;
          need_to_cancel_cam_transform = true;
          astate->mouse_distance_pp = 0.0f;
        }

      if (event->button() == Qt::MiddleButton) {
          astate->mouse_md_pressed = false;
          need_to_cancel_cam_transform = true;
        }

      if (astate->camera && need_to_cancel_cam_transform) {
          astate->camera->m_rotate_camera = false;
          astate->camera->m_move_camera = false;
        }

    }
}

void ws_viewer_widget_t::mouseMoveEvent(QMouseEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  if (event) {

      astate->mouse_x_old = astate->mouse_x;
      astate->mouse_y_old = astate->mouse_y;
      astate->mouse_x_dc_old = astate->mouse_x_dc;
      astate->mouse_y_dc_old = astate->mouse_y_dc;
      astate->mouse_x = event->x();
      astate->mouse_y = event->y();
      //      mouse_x_ws_frame = (mouse_x_ws_frame / viewport_size_c(0)-0.5)*2.0;
      //      mouse_y_ws_frame = (0.5 - mouse_y_ws_frame / viewport_size_c(1))*2.0;
      astate->mouse_x_dc = (astate->mouse_x / float(this->width()) - 0.5f) * 2.0f;
      astate->mouse_y_dc = (0.5f - astate->mouse_y / float(this->height())) * 2.0f;

      //      astate->mouse_x_dc_old = (astate->mouse_x_old / float(this->width()) - 0.5f) * 2.0f;
      //      astate->mouse_y_dc_old = (0.5f - astate->mouse_y_old / float(this->height())) * 2.0f;

      //      astate->log(fmt::format("{} {} {} {} {} {} {}",  astate->is_mouse_moving,
      //                              astate->mouse_x_dc, astate->mouse_y_dc,
      //                              astate->mouse_x_dc_old, astate->mouse_y_dc_old,
      //                              fabs(astate->mouse_x_dc - astate->mouse_x_dc_old),
      //                              fabs(astate->mouse_y_dc - astate->mouse_y_dc_old) ));

      astate->is_mouse_moving = (abs(astate->mouse_x_dc - astate->mouse_x_dc_old) > 0.001f ||
                                 abs(astate->mouse_y_dc - astate->mouse_y_dc_old) > 0.001f);

      astate->mouse_distance_pp += abs(astate->mouse_x - astate->mouse_x_old) +
                                   abs(astate->mouse_y - astate->mouse_y_old);

      if (!astate->is_mouse_moving) {
          astate->mouse_distance_pp = 0;
        }
      //if (astate->mouse_lb_pressed)

      //      std::cout << fmt::format("{} {} {} {} \n",
      //                               astate->mouse_x_old,
      //                               astate->mouse_y_old,
      //                               astate->mouse_x,
      //                               astate->mouse_y)<< std::endl;

      //float delta =
      if (astate->camera && astate->mouse_rb_pressed) {
          astate->camera->m_rotate_camera = true;
          astate->make_viewport_dirty();
        }

      if (astate->camera && astate->mouse_md_pressed) {
          astate->camera->m_move_camera = true;
          astate->make_viewport_dirty();
        }

      if (astate->camera && !astate->is_mouse_moving) {
          astate->camera->m_rotate_camera = false;
          astate->camera->m_move_camera = false;
        }
    }

  //fmt::print(std::cout, "mouseMoveEvent\n");
}

void ws_viewer_widget_t::wheelEvent(QWheelEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->camera) {
      astate->camera->update_camera_zoom(event->delta() / 180.0f);
      astate->camera->update_camera();
      astate->make_viewport_dirty();
    }

}

void ws_viewer_widget_t::show_context_menu(const QPoint &pos) {

//  app_state_t* astate = app_state_t::get_inst();

//  astate->log(fmt::format("DEBUG MOUSE_DISTANCE_PP {}", astate->mouse_distance_pp));
//  if (astate->mouse_distance_pp < 30.2f) {
//      astate->mouse_lb_pressed = false;
//      astate->mouse_rb_pressed = false;
//      astate->mouse_md_pressed = false;
//      astate->is_mouse_moving = false;
//      astate->mouse_distance_pp = 0.0f;

//      QPoint globalPos = mapToGlobal(pos);

//      // Create menu and insert some actions
//      QMenu myMenu;
//      myMenu.addAction("Insert");
//      myMenu.addAction("Erase");

//      // Show context menu at handling position
//      myMenu.exec(globalPos);
//    }
}
