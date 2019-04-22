#include <qppcad/ws_item/node_book/qnode_socket_colorize.hpp>

using namespace qpp;
using namespace qpp::cad;

QColor sck_colorize_helper::get_color(sflow_parameter_e par_type) {

  switch (par_type) {

    case sflow_parameter_e::sfpar_none :
      return Qt::gray;
      break;

    case sflow_parameter_e::sfpar_int :
      return Qt::blue;
      break;

    case sflow_parameter_e::sfpar_bool :
      return Qt::green;
      break;

    case sflow_parameter_e::sfpar_float :
      return Qt::red;
      break;

    case sflow_parameter_e::sfpar_string :
      return Qt::white;
      break;

    case sflow_parameter_e::sfpar_v3f :
      return Qt::yellow;
      break;

    case sflow_parameter_e::sfpar_xgeom :
      return Qt::darkMagenta;
      break;
    }

  return Qt::darkBlue;

}
