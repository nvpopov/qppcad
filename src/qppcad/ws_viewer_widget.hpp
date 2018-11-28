#ifndef QPP_CAD_WS_VIEWER_WIDGET_H
#define QPP_CAD_WS_VIEWER_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>

namespace qpp {

  namespace cad {

    class ws_viewer_widget_t : public QOpenGLWidget {
      public:
        ws_viewer_widget_t(QWidget *parent);
        QTimer *m_update_timer;

      public slots:
        void update_cycle();
        void show_context_menu(const QPoint &pos);

      protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        void mousePressEvent(QMouseEvent * event) override;
        void mouseReleaseEvent(QMouseEvent * event) override;
        void mouseMoveEvent(QMouseEvent * event) override;
        void wheelEvent(QWheelEvent *event) override;

    };

  }

}
#endif
