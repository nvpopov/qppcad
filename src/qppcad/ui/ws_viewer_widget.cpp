#include <qppcad/ui/ws_viewer_widget.hpp>
#include <qppcad/app_state.hpp>
#include <QApplication>

using namespace qpp;
using namespace qpp::cad;

ws_viewer_widget_t::ws_viewer_widget_t(QWidget *parent) : QOpenGLWidget (parent) {

  m_update_timer = new QTimer;

  m_update_timer->connect(m_update_timer,
                          &QTimer::timeout,
                          this,
                          &ws_viewer_widget_t::update_cycle);

  m_update_timer->setTimerType(Qt::PreciseTimer);
  m_update_timer->start(16);

  setMouseTracking(true);
  setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

  connect(this,
          &ws_viewer_widget_t::customContextMenuRequested,
          this,
          &ws_viewer_widget_t::show_context_menu);

  setMinimumHeight(300);
  setMinimumWidth(300);

  m_update_timer_cpu = new QElapsedTimer;
  m_update_timer_gpu = new QElapsedTimer;

}

void ws_viewer_widget_t::draw_text_logo(QPainter &painter) {

  app_state_t* astate = app_state_t::get_inst();

  QFont font(QFont(astate->m_font_name, 42, QFont::Medium));
  painter.setFont(font);
  QFontMetrics fm(font);
  QColor logo_color = QColor::fromRgb(75, 75, 75);

  QString logo_string = tr("qpp::cad, git rev.:%1")
                        .arg(QString::fromStdString(build_info_helper::get_git_version()));

  int c_x = painter.window().width() - fm.width(logo_string) - 10;
  int c_y = painter.window().height() - fm.ascent() + 20;

  painter.setPen(logo_color);
  painter.drawText(c_x, c_y, logo_string);

}

void ws_viewer_widget_t::update_cycle() {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->is_viewport_dirty()) {

      astate->cleanup_viewport();

      if (astate->camera && (astate->camera->m_rotate_camera || astate->camera->m_move_camera)) {
          astate->camera->update_camera();
          astate->camera->m_rotate_camera = false;
          astate->camera->m_move_camera = false;
        }

      update();

    }

  if (astate->ws_mgr->has_wss()) {
      m_update_timer_cpu->start();
      auto cur_ws = astate->ws_mgr->get_cur_ws();
      if (cur_ws) cur_ws->update(0.016f);
      astate->m_last_frame_time_cpu = m_update_timer_cpu->nsecsElapsed();
    }

  if (astate->mouse_rb_pressed) {
      astate->make_viewport_dirty();
    }

  astate->is_mouse_moving = false;

  astate->ws_mgr->utility_event_loop();

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
  if (astate->camera) astate->camera->update_camera();

}

void ws_viewer_widget_t::paintGL() {

  m_update_timer_gpu->start();

  app_state_t* astate = app_state_t::get_inst();
  glapi_t* glapi = astate->glapi;

  if (astate->m_disable_app) return;

  glapi->glViewport(static_cast<int>(0),
                    static_cast<int>(0),
                    static_cast<int>(astate->viewport_size(0)),
                    static_cast<int>(astate->viewport_size(1)));

  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  astate->dp->depth_func(draw_pipeline_depth_func::depth_enabled);
  astate->dp->depth_func(draw_pipeline_depth_func::depth_lequal);
  astate->dp->cull_func(draw_pipeline_cull_func::cull_enable);
  astate->dp->cull_func(draw_pipeline_cull_func::cull_back);
  astate->ws_mgr->render_cur_ws();
  astate->dp->depth_func(draw_pipeline_depth_func::depth_disabled);
  astate->dp->cull_func(draw_pipeline_cull_func::cull_disable);

  if (astate->m_show_debug_frame_stats) {

      painter.setFont(QFont(astate->m_font_name, 13));

      //build contrast color for font
      QColor debug_hud_color = Qt::black;
      if (astate->ws_mgr->has_wss()) {
          auto cur_ws = astate->ws_mgr->get_cur_ws();
          debug_hud_color.setRedF(1 - cur_ws->m_background_color[0]);
          debug_hud_color.setGreenF(1 - cur_ws->m_background_color[1]);
          debug_hud_color.setBlueF(1 - cur_ws->m_background_color[2]);
        }

      painter.setPen(debug_hud_color);
      painter.drawText(3, 3, 280, 30, Qt::AlignLeft,
                       QString::fromStdString(fmt::format("Frame time GPU: {:6.6f} ms.",
                                                          (astate->m_last_frame_time_gpu)/
                                                          1000000.0))
                       );

      painter.drawText(3, 30, 280, 30, Qt::AlignLeft,
                       QString::fromStdString(fmt::format("Frame time CPU: {:6.6f} ms.",
                                                          (astate->m_last_frame_time_cpu)/
                                                          1000000.0))
                       );
    }

  astate->ws_mgr->render_cur_ws_overlay(painter);

  if (!astate->ws_mgr->has_wss()) draw_text_logo(painter);

  painter.end();
  astate->m_last_frame_time_gpu = m_update_timer_gpu->nsecsElapsed();

}

void ws_viewer_widget_t::mousePressEvent(QMouseEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  if (event) {

      if (event->button() == Qt::LeftButton)  {
          astate->mouse_lb_pressed = true;
          astate->ws_mgr->mouse_click();
          astate->mouse_distance_pp = 0.0f;
        }

      if (event->button() == Qt::RightButton) {
          astate->mouse_rb_pressed = true;

        }

      if (event->button() == Qt::MiddleButton) astate->mouse_md_pressed = true;
    }

}

void ws_viewer_widget_t::mouseReleaseEvent(QMouseEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  if (event) {

      bool need_to_cancel_cam_transform = false;

      if (event->button() == Qt::LeftButton) {
          astate->mouse_lb_pressed = false;
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

      astate->mouse_x_dc = (astate->mouse_x / float(this->width()) - 0.5f) * 2.0f;
      astate->mouse_y_dc = (0.5f - astate->mouse_y / float(this->height())) * 2.0f;


      astate->is_mouse_moving = (abs(astate->mouse_x - astate->mouse_x_old) > 0 ||
                                 abs(astate->mouse_y - astate->mouse_y_old) > 0);

      astate->mouse_distance_pp += abs(astate->mouse_x - astate->mouse_x_old) +
                                   abs(astate->mouse_y - astate->mouse_y_old);

      if (!astate->is_mouse_moving) {
          astate->mouse_distance_pp = 0;
        }

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
          astate->camera->m_rotate_over = false;
        }

      Qt::KeyboardModifiers kb_mod = QApplication::keyboardModifiers();
      if (astate->camera) {
          astate->camera->m_rotate_over =
              astate->camera->m_rotate_camera && (kb_mod & Qt::ControlModifier);
        }
    }

}

void ws_viewer_widget_t::wheelEvent(QWheelEvent *event) {

  app_state_t* astate = app_state_t::get_inst();

  if (astate->camera) {
      astate->camera->update_camera_zoom(
            event->delta() / 180.0f * astate->m_middle_mb_translate_mode);
      astate->camera->update_camera();
      astate->make_viewport_dirty();
    }

}

void ws_viewer_widget_t::show_context_menu(const QPoint &pos) {

//  QMenu contextMenu(tr("Context menu"), this);
//  QAction action1("Action 1", this);
//  contextMenu.addAction(&action1);
//  contextMenu.exec(mapToGlobal(pos));

//  app_state_t* astate = app_state_t::get_inst();
//  astate->is_mouse_moving = false;
//  astate->mouse_lb_pressed = false;
//  astate->mouse_rb_pressed = false;
//  astate->mouse_rb_pressed = false;

}
