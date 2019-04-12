#ifndef QPPCAD_GLAPI_H
#define QPPCAD_GLAPI_H

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

namespace qpp {

  namespace cad {

    class glapi_t : public QOpenGLFunctions_3_3_Core {
      public:
        glapi_t();
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
