#include <qppcad/core/gitversion.h>
#include <qppcad/core/gitversion_ex.h>

using namespace qpp;
using namespace qpp::cad;

std::string build_info_helper::get_git_version() {
  return GIT_REVISION;
}

std::string build_info_helper::get_build_date() {
  return BUILD_TIMESTAMP;
}
