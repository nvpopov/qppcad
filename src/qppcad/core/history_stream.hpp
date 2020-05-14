#ifndef QPPCAD_HISTORY_STREAM
#define QPPCAD_HISTORY_STREAM

#include <qppcad/core/qppcad.hpp>

namespace qpp {

namespace cad {

class hist_doc_base_t;
using epoch_t = std::size_t;

enum hist_doc_delta_state_e {
  delta_instant,
  delta_incremental
};

enum hr_result_e {
  hr_error = 0,
  hr_success = 1,
  hr_invalid_epoch = 2,
  hr_epoch_ill_defined = 3,
  hr_invalid_child = 4,
  hr_invalid_child_epoch = 5,
  hr_true = 6,
  hr_false = 7
};

//Stores augment meta info for each child in each parent's epoch
struct hs_child_state_meta_t {
  epoch_t m_child_epoch{0};
  bool m_is_alive{false};
};

/**
* @brief The hist_doc_base_t class
*/
class hist_doc_base_t {

public:

  using epoch_t = size_t;
  using self_t = hist_doc_base_t;

private:

  epoch_t p_cur_epoch{0};
  self_t *p_parent{nullptr};
  std::vector<self_t*> p_childs;
  std::map<epoch_t, std::map<self_t*, hs_child_state_meta_t>> p_childs_states;
  std::map<self_t*, hs_child_state_meta_t> p_current_child_state;
  std::vector<epoch_t> p_hist_line{0};
  bool p_is_bad{false};

  hist_doc_delta_state_e p_dstate{hist_doc_delta_state_e::delta_instant};

protected:

  virtual hr_result_e reset_impl();
  virtual bool is_unmodified_impl();

public:

  hist_doc_base_t ();
  virtual ~hist_doc_base_t();

  /**
  * @brief get_cur_epoch
  * @return
  */
  epoch_t get_cur_epoch();

  /**
  * @brief set_cur_epoch
  * @param cur_epoch
  * @return
  */
  hr_result_e set_cur_epoch(epoch_t cur_epoch);

  virtual hr_result_e on_epoch_changed(epoch_t prev_epoch);
  virtual hr_result_e on_epoch_removed(epoch_t target_epoch);

  hr_result_e commit_exclusive(hist_doc_base_t *child = nullptr,
                               std::optional<epoch_t> child_epoch = std::nullopt);
  virtual void on_commit_exclusive();

  /**
  * @brief get_delta_state_type - not implemented yet
  * @return
  */
  hist_doc_delta_state_e get_delta_state_type();

  /**
  * @brief set_delta_state_type - not implemented yet
  * @param new_dstate
  */
  void set_delta_state_type(hist_doc_delta_state_e new_dstate);

  /**
  * @brief is_dirty - not implemented yet
  * @return
  */
  bool is_unmodified();

  hr_result_e reset();

  /**
  * @brief push_epoch insert new epoch at cursor
  * @param new_epoch
  * @return
  */
  std::tuple<hr_result_e, std::optional<epoch_t>> push_epoch(
      std::optional<epoch_t> new_epoch_ex = std::nullopt,
      bool checkout_to_new_epoch = false);

  auto find_hl(epoch_t target) {
    return std::find(begin(p_hist_line), end(p_hist_line), target);
  }

  /**
  * @brief get_history_size
  */
  size_t get_history_size();
  std::vector<epoch_t> get_history();

  /**
  * @brief augment_epoch
  * @param child pointer to child
  * @param child_epoch
  * @param target_epoch
  */
  hr_result_e augment_epoch(epoch_t target_epoch, self_t* child, epoch_t child_epoch,
                            bool alive = true);

  /**
  * @brief get_augmented_count
  * @param target_epoch
  * @return
  */
  size_t get_augmented_count(epoch_t target_epoch);

  /**
  * @brief remove_augment_from_epoch
  * @param child
  * @param child_epoch
  * @param target_epoch
  * @return
  */
  hr_result_e remove_augment_from_epoch(self_t* child, epoch_t target_epoch);
  hr_result_e is_child_alive(epoch_t target_epoch, self_t* child);

  /**
  * @brief has_epoch
  * @param target_epoch
  * @return
  */
  bool has_epoch(epoch_t target_epoch);

  /**
  * @brief checkout_to_epoch
  * @param target_epoch
  * @return
  */
  hr_result_e checkout_to_epoch(epoch_t target_epoch);
  hr_result_e checkout_by_dist(int dist);
  bool can_checkout_by_dist(int dist);

  /**
  * @brief add_child
  * @param child
  * @return
  */
  hr_result_e add_hs_child(self_t *child);

  /**
  * @brief get_root
  * @return
  */
  self_t *get_root();

  /**
  * @brief get_children
  * @param idx
  * @return
  */
  self_t *get_children(size_t idx);

  /**
  * @brief is_children
  * @param child
  * @return
  */
  std::optional<size_t> is_children(self_t *child);

  /**
  * @brief remove_child
  * @param child_id
  */
  hr_result_e remove_child(size_t child_id);

  /**
  * @brief remove_child
  * @param child
  */
  hr_result_e remove_child(self_t *child);

  /**
  * @brief get_children_count
  */
  size_t get_children_count();
  //end of children stuff

};

template<typename STYPE>
class hist_doc_t : public hist_doc_base_t {

private:

  STYPE p_cur_value{};
  std::map<epoch_t, STYPE> p_stored_values;

protected:

  hr_result_e reset_impl() override {
    if (p_stored_values.empty()) return hr_result_e::hr_invalid_epoch;
    auto stored_val_it = p_stored_values.find(get_cur_epoch());
    if (stored_val_it == end(p_stored_values)) return hr_result_e::hr_invalid_epoch;
    p_cur_value = stored_val_it->second;
    return hr_result_e::hr_success;
  }

public:

  hist_doc_t(STYPE &&new_val) {
    p_stored_values[get_cur_epoch()] = new_val;
    p_cur_value = p_stored_values[get_cur_epoch()];
  }

  hist_doc_t() {
    p_stored_values[get_cur_epoch()] = p_cur_value;
  }

  hr_result_e push_epoch_with_value(STYPE &&new_val,
                                    std::optional<epoch_t> new_epoch = std::nullopt,
                                    bool checkout_to_new_epoch = false) {

    auto push_epoch_res = push_epoch(new_epoch, checkout_to_new_epoch);
    if (std::get<0>(push_epoch_res) == hr_result_e::hr_success) {
      p_stored_values[*std::get<1>(push_epoch_res)] = new_val;
      return hr_result_e::hr_success;
    } else {
      return hr_result_e::hr_error;
    }

  }

  void on_commit_exclusive() override {
    p_stored_values[get_cur_epoch()] = p_cur_value;
  }

  hr_result_e commit_value_exclusive(STYPE &&new_val,
                                     std::optional<epoch_t> new_epoch = std::nullopt) {

    STYPE loc_var(new_val);
    auto push_epoch_with_value_res =
        push_epoch_with_value(std::move(loc_var), new_epoch, true);

    if (push_epoch_with_value_res != hr_result_e::hr_success)
      return hr_result_e::hr_error;

    commit_exclusive();

    return hr_result_e::hr_success;

  }

  hr_result_e on_epoch_changed(epoch_t prev_epoch) override {

    auto cur_epoch = get_cur_epoch();
    auto val_it = p_stored_values.find(cur_epoch);
    if (val_it != p_stored_values.end()) {
      p_cur_value = val_it->second;
      return hr_result_e::hr_success;
    } else {
      return hr_result_e::hr_error;
    }

  }

  hr_result_e on_epoch_removed(epoch_t epoch_to_remove) override {

    auto it = p_stored_values.find(epoch_to_remove);
    if (it != p_stored_values.end()) {
      p_stored_values.erase(it);
      return hr_result_e::hr_success;
    } else {
      return hr_result_e::hr_error;
    }

  }

  bool is_unmodified_impl() override {
    if (p_stored_values.empty()) return false;
    auto stored_val_it = p_stored_values.find(get_cur_epoch());
    if (stored_val_it == end(p_stored_values)) return false;
    return p_cur_value == stored_val_it->second;
  }

  STYPE get_value() const {
    return p_cur_value;
  }

  void set_value(STYPE &&new_val) {
    p_cur_value = new_val;
  }

  void set_cvalue(STYPE new_val) {
    p_cur_value = new_val;
  }

};

template<typename STYPE>
class hist_property_t : public hist_doc_t<STYPE> {

};

using hs_prop_int_t     = hist_property_t<int>;
using hs_prop_bool_t    = hist_property_t<bool>;
using hs_prop_float_t   = hist_property_t<float>;
using hs_prop_double_t  = hist_property_t<double>;
using hs_prop_str_t     = hist_property_t<std::string>;
using hs_prop_v3f_t     = hist_property_t<vector3<float>>;
using hs_prop_v3d_t     = hist_property_t<vector3<double>>;
using hs_prop_v3i_t     = hist_property_t<vector3<int>>;
using hs_prop_v2f_t     = hist_property_t<vector2<float>>;
using hs_prop_v2d_t     = hist_property_t<vector2<double>>;
using hs_prop_v2i_t     = hist_property_t<vector3<int>>;
using hs_prop_m3f_t     = hist_property_t<matrix3<float>>;
using hs_prop_m3d_t     = hist_property_t<matrix3<double>>;

} // namespace qpp::cad

} // namespace qpp

#endif
