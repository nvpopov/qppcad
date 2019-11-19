#ifndef QPPCAD_IUPDATABLE
#define QPPCAD_IUPDATABLE

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    class iupdatable_externally_t {

      public:

        virtual void updated_externally(uint32_t update_reason) = 0;

    };

    class iupdatable_t {

      public:

        virtual void updated(float deltatime) = 0;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
