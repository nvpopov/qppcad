#include <qppcad/uniq_id.hpp>
#include <qppcad/app.hpp>

using namespace qpp::cad;

  static uint32_t uniq_id_storage = 0;

  uint32_t uniq_id_provider::get_uniq_id(){
    uniq_id_storage += 1;
    c_app::log(fmt::format("New uniq id requested({})", uniq_id_storage));
    return uniq_id_storage;
  }

