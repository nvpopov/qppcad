#ifndef QPPCAD_QNODE_SOCKET_COLORIZE
#define QPPCAD_QNODE_SOCKET_COLORIZE

#include <qppcad/core/qppcad.hpp>
#include <qppcad/sflow/sflow_parameter.hpp>
#include <QColor>

namespace qpp {

  namespace cad {

    struct sck_colorize_helper {
        static QColor get_color(sf_parameter_e par_type);
    };

  }

}

#endif
