#include <qppcad/core/gitversion.h>
#include <qppcad/core/gitversion_ex.h>

using namespace qpp;
using namespace qpp::cad;

std::string build_info_t::get_git_version() {
  return GIT_REVISION;
}

std::string build_info_t::get_build_date() {
  return BUILD_TIMESTAMP;
}

std::string build_info_t::get_qt_version() {
  return QPPCAD_QT_BUILD_VERSION;
}

std::string build_info_t::get_python_version() {
  return QPPCAD_PYTHON_VERSION;
}

bool build_info_t::get_is_debug() {
#ifdef QPPCAD_DEBUG
  return true;
#else
  return false;
#endif
}
