#ifndef HISTORY_STREAM_ARRAY_PROXY
#define HISTORY_STREAM_ARRAY_PROXY

#include <qppcad/core/history_stream.hpp>
#include <cassert>

namespace qpp {

namespace cad {

template<typename STYPE>
class hist_doc_array_proxy_t : public hist_doc_base_t {

private:

  std::vector<STYPE*> p_array_data;
  std::map<hist_doc_base_t*, STYPE*> p_map_hs_to_array;

public:

  static_assert(std::is_base_of<hist_doc_base_t, STYPE>::value);

  hs_result_e add_hs_child_as_array(STYPE *new_arr_element) {
    hist_doc_base_t *as_hsd = reinterpret_cast<hist_doc_base_t*>(new_arr_element);
    if (!as_hsd) return hs_result_e::hs_error;
    p_map_hs_to_array[as_hsd] = new_arr_element;
    return add_hs_child(as_hsd, true);
  }

  STYPE* get_hs_child_as_array(size_t chl_idx) {
    hist_doc_base_t *elem = get_hs_child(chl_idx);
    if (!elem) return nullptr;
    auto map_itr = p_map_hs_to_array.find(elem);
    return map_itr != end(p_map_hs_to_array) ? map_itr->second : nullptr;
  }

};

}

}

#endif
