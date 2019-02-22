#include <qppcad/render/glapi.hpp>

using namespace qpp;
using namespace qpp::cad;

glapi_t::glapi_t() {
  initializeOpenGLFunctions();
}
