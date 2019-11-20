#ifndef QPPCAD_SERIALIZABLE_H
#define QPPCAD_SERIALIZABLE_H

#include <qppcad/core/json_adapter.hpp>

namespace qpp {

  namespace cad {

    class serializable_t {

      public:

        virtual void save_to_json(json &data) {}
        virtual void load_from_json(json &data) {}

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
