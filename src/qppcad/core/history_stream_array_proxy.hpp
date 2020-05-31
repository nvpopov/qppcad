#ifndef HISTORY_STREAM_ARRAY_PROXY
#define HISTORY_STREAM_ARRAY_PROXY

#include <qppcad/core/history_stream.hpp>
#include <cassert>

namespace qpp {

namespace cad {

template<typename T>
struct promote_to_value_type {
  using type =
      typename std::conditional<std::is_pointer<T>::value, std::remove_pointer<T>, T>::type;
};

template<typename STYPE>
struct hs_doc_array_proxy_raw_pointer_policy {

  using holder_type_t =
      typename std::add_pointer<typename promote_to_value_type<STYPE>::type>::type;
  static void delete_holder(holder_type_t &elem) {
    delete elem;
    elem = nullptr;
  }

};

template<typename STYPE>
struct hs_doc_array_proxy_shared_pointer_policy {

  using holder_type_t =
      typename std::shared_ptr<typename promote_to_value_type<STYPE>::type>::type;
  static void delete_holder(holder_type_t elem) {

  }

};

template<typename STYPE,
         typename STYPE_STORAGE_POLICY = hs_doc_array_proxy_raw_pointer_policy<STYPE>>
class hist_doc_array_proxy_t : public hist_doc_base_t {

public:

  using holder_type_t = typename STYPE_STORAGE_POLICY::holder_type_t;

private:

  std::vector<holder_type_t> p_array_data;
  std::map<hist_doc_base_t*, holder_type_t> p_map_hs_to_array;

public:

  static_assert(std::is_base_of<hist_doc_base_t, STYPE>::value);

  hs_result_e add_hs_child_as_array(holder_type_t new_arr_element) {

    hist_doc_base_t *as_hsd = reinterpret_cast<hist_doc_base_t*>(new_arr_element);
    if (!as_hsd) return hs_result_e::hs_error;

    p_array_data.push_back(new_arr_element);
    p_map_hs_to_array[as_hsd] = new_arr_element;
    return add_hs_child(as_hsd, true);

  }

  holder_type_t get_hs_child_as_array(size_t chl_idx) {

    hist_doc_base_t *elem = get_hs_child(chl_idx);
    if (!elem) return nullptr;

    auto map_itr = p_map_hs_to_array.find(elem);
    return map_itr != end(p_map_hs_to_array) ? map_itr->second : nullptr;

  }

  void request_child_deletion(hist_doc_base_t *child) override {

    auto map_itr = p_map_hs_to_array.find(child);
    if (map_itr == end(p_map_hs_to_array)) return;

    holder_type_t childw = map_itr->second;
    p_map_hs_to_array.erase(map_itr);

    auto arr_data_it = std::find(begin(p_array_data), end(p_array_data), childw);
    if (arr_data_it != end(p_array_data)) p_array_data.erase(arr_data_it);

    STYPE_STORAGE_POLICY::delete_holder(childw);

  }

};

}

}

#endif
