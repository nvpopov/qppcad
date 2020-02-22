#include <qppcad/ws_item/node_book/qnode_socket_colorize.hpp>

using namespace qpp;
using namespace qpp::cad;

QColor sck_colorize_helper::get_color(sf_parameter_e par_type) {

  switch (par_type) {

    case sf_parameter_e::sfpar_none :
      return Qt::gray;
      break;

    case sf_parameter_e::sfpar_int :
      return Qt::blue;
      break;

    case sf_parameter_e::sfpar_bool :
      return Qt::green;
      break;

    case sf_parameter_e::sfpar_float :
      return Qt::red;
      break;

    case sf_parameter_e::sfpar_string :
      return Qt::white;
      break;

    case sf_parameter_e::sfpar_v3f :
      return Qt::yellow;
      break;

    case sf_parameter_e::sfpar_xgeom :
      return Qt::darkMagenta;
      break;

    case sf_parameter_e::sfpar_ws_item :
      return Qt::black;
      break;
    }

  return Qt::darkBlue;

}
