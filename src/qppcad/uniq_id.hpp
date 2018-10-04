#ifndef QPPCAD_UNIQ_ID
#define QPPCAD_UNIQ_ID
#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    struct uniq_id_provider {
      static uint32_t get_uniq_id();
    };

  }

}

#endif
