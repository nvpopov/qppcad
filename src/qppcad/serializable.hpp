#ifndef QPPCAD_SERIALIZABLE_H
#define QPPCAD_SERIALIZABLE_H

#include <qppcad/json_adapter.hpp>

namespace qpp {

    namespace cad {

        class serializable_t {

            public:

                virtual void save_to_json(json &data) = 0;
                virtual void load_from_json(json &data) = 0;

        };

    } // namespace qpp::cad

  } // namespace qpp

  #endif
