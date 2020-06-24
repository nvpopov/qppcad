#ifndef QPPCAD_GITVERSION
#define QPPCAD_GITVERSION

#include <string>

namespace qpp {

  namespace cad {

    class build_info_t {

      public:

        static std::string get_git_version();
        static std::string get_build_date();
        static std::string get_qt_version();
        static std::string get_python_version();
        static bool get_is_debug();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
