#ifndef QPPCAD_PLATFORM_GUARD
#define QPPCAD_PLATFORM_GUARD

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    struct platform_helper_t {

      static size_t get_mem_usage_mb();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
