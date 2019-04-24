#include <qppcad/type_info.hpp>
#include <qppcad/string_hash_register.hpp>

using namespace qpp;
using namespace qpp::cad;

qtype_info::qtype_info(std::string type_name,
                       const qtype_info *base_type_info) {

  p_type_name = type_name;
  p_type_hash = string_hash_register_t::calc_hash_ub(type_name);
  p_base_type_info = base_type_info;

}

qtype_info::~qtype_info() = default;

bool qtype_info::is_type_of(size_t type) const {

  const qtype_info* current = this;

  while (current) {
      if (current->get_type() == type)
        return true;
      current = current->get_base_type_info();
    }

  return false;
}

bool qtype_info::is_type_of(const qtype_info *_typeInfo) const {

  const qtype_info* current = this;

  while (current) {
      if (current == _typeInfo)
        return true;

      current = current->get_base_type_info();
    }

  return false;
}
