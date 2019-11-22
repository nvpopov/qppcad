#ifndef QPPCAD_PROPERTY
#define QPPCAD_PROPERTY

#include <qppcad/core/qppcad.hpp>
#include <qppcad/core/qpp_object.hpp>

namespace qpp {

  namespace cad {

    class generalized_property_t : public qpp_object_t {

      QPP_OBJECT(generalized_property_t, qpp_object_t)

      public:

        qpp_object_t *m_owner{nullptr};
        virtual bool is_trivially_changes_state() {return true;}

    };

  }

}

#endif
