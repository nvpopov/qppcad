#ifndef QPPCAD_WS_VIEWER_WIDGET_H
#define QPPCAD_WS_VIEWER_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>

namespace qpp {

  namespace cad {

    class ws_viewer_widget_t : public QOpenGLWidget {

      Q_OBJECT

      public:

        ws_viewer_widget_t(QWidget *parent);
        QTimer *m_update_timer;
        QElapsedTimer *m_update_timer_gpu;
        QElapsedTimer *m_update_timer_cpu;

        void draw_text_logo(QPainter &painter);
        void draw_scanline(QPainter &painter);

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

  } // namespace qpp::cad

} // namespace qpp

#endif
