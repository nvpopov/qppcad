#ifndef QPP_CAD_WS_VIEWER_WIDGET_H
#define QPP_CAD_WS_VIEWER_WIDGET_H

#include <qppcad/qppcad.hpp>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

namespace qpp {

  namespace cad {

    class ws_viewer_widget_t : public QOpenGLWidget {
      public:
        ws_viewer_widget_t(QWidget *parent);

      protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
    };

  }

}
#endif
