#include <qppcad/string_hash_register.hpp>

using namespace qpp;
using namespace qpp::cad;

string_hash_register_t::string_hash_register_t() {

}

bool string_hash_register_t::register_string(const std::string &_str,
                                             const size_t _hash) {
  auto it = p_data.find(_hash);

  if (it != p_data.end()) {
      return (it->second == _str);
    }
  else {
      p_data.insert(std::pair<size_t, std::string>(_hash, _str));
      return true;
    }
}

bool string_hash_register_t::register_string(const std::string &_str) {

  size_t _hash = calc_hash_ub(_str);
  return register_string(_str, _hash);
}

std::string string_hash_register_t::get_pretty_string(const size_t _hash) {
  auto it = p_data.find(_hash);
  if (it != p_data.end()) return it->second;
  else return "ERROR";
}

std::optional<std::string> string_hash_register_t::get_string(const size_t _hash) {
  auto it = p_data.find(_hash);
  if (it != p_data.end()) return std::optional<std::string>(it->second);
  else return std::nullopt;
}

std::optional<size_t> string_hash_register_t::get_hash(const std::string &_str) {
  for (auto &elem : p_data)
    if (elem.second == _str) return std::optional<size_t>(elem.first);
  return std::nullopt;
}

size_t string_hash_register_t::calc_hash(const std::string &_str) {
  for (auto &elem : p_data)
    if (elem.second == _str) return elem.first;
  return calc_hash_ub(_str);
}

bool string_hash_register_t::contains(size_t _hash) {
  auto it = p_data.find(_hash);
  return it != p_data.end();
}

bool string_hash_register_t::contains(const std::string &_str) {
  for (auto &elem : p_data)
    if (elem.second == _str) return true;
  return false;
}
