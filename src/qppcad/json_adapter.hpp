#ifndef QPPCAD_JSON_ADAPTER
#define QPPCAD_JSON_ADAPTER
#include <qppcad/qppcad.hpp>
#include <fifo_map.hpp>
#include <nlohmann/json.hpp>

namespace qpp::cad{
  template<class K, class V, class dummy_compare, class A>
  using workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

  using json =
  nlohmann::basic_json<workaround_fifo_map,
  std::vector,
  std::string,
  bool,
  std::int64_t,
  std::uint64_t,
  float>;
}
#endif
