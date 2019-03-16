#ifndef QPPCAD_GITVERSION
#define QPPCAD_GITVERSION

#include <string>

namespace qpp {

  namespace cad {

    class build_info_helper {

      public:

        static std::string get_git_version();
        static std::string get_build_date();

    };

  }

}

#endif
