#include <qppcad/type_info.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

type_info::type_info(std::string type_name,
                     const type_info *base_type_info) {

  p_type_name = type_name;
  p_type_hash = app_state_t::get_inst()->hash_reg->calc_hash(type_name);
  p_base_type_info = base_type_info;

}

type_info::~type_info() = default;

bool type_info::is_type_of(size_t type) const {

  const type_info* current = this;

  while (current) {
      if (current->get_type() == type)
        return true;
      current = current->get_base_type_info();
    }

  return false;
}

bool type_info::is_type_of(const type_info *_typeInfo) const {

  const type_info* current = this;

  while (current) {
      if (current == _typeInfo)
        return true;

      current = current->get_base_type_info();
    }

  return false;
}
