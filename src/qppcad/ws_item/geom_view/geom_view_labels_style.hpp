#ifndef QPPCAD_GEOM_VIEW_LABELS_STYLE
#define QPPCAD_GEOM_VIEW_LABELS_STYLE

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    enum geom_labels_style_e : int {

      show_none,
      show_id,
      show_type,
      show_id_type,
      show_charge,
      show_charge_type,
      show_charge_id_type,
      show_custom

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
