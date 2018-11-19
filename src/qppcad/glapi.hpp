#ifndef QPP_CAD_GLAPI_H
#define QPP_CAD_GLAPI_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

namespace qpp {

  namespace cad {

    class glapi_t : public QOpenGLFunctions_3_3_Core {
      public:
        glapi_t();
    };

  }

}


#endif
