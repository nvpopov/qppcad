#include <qppcad/core/qpp_object.hpp>

using namespace qpp;
using namespace qpp::cad;


bool qpp_object_t::is_instance_of(size_t type) const {
  return get_type_info()->is_type_of(type);
}

bool qpp_object_t::is_instance_of(const qtype_info *_type_info) const {
  return get_type_info()->is_type_of(_type_info);
}
