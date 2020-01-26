#ifndef QPPCAD_PROPERTY
#define QPPCAD_PROPERTY

#include <qppcad/core/qppcad.hpp>
#include <qppcad/core/qpp_object.hpp>
#include <stack>

namespace qpp {

  namespace cad {

    class generalized_property_t : public qpp_object_t {

      QPP_OBJECT(generalized_property_t, qpp_object_t)

      public:

        qpp_object_t *m_owner{nullptr};
        virtual bool is_trivially_changes_state() {return true;}

    };

    template<typename T>
    struct property_record_t {
      size_t m_epoch;
      T m_value;
    };

    template<typename T>
    class property_t: public generalized_property_t {

    public:

      std::stack<property_record_t<T> > m_data;

    };

    class bool_property_t : public property_t<bool> {

      QPP_OBJECT(bool_property_t, generalized_property_t)

    };

  }

}

#endif
