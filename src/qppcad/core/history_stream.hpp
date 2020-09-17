#ifndef QPPCAD_HISTORY_STREAM
#define QPPCAD_HISTORY_STREAM

#include <qppcad/core/qppcad.hpp>

namespace qpp {

namespace cad {

class hs_doc_base_t;
using epoch_t = std::size_t;

enum class hs_dstate_e {
  hs_dstate_inst, /* instant */
  hs_dstate_incr  /* incremental */
};

enum class hs_result_e {
  hs_error = 0,
  hs_success = 1,
  hs_invalid_epoch = 2,
  hs_epoch_ill_defined = 3,
  hs_invalid_child = 4,
  hs_invalid_child_epoch = 5,
  hs_true = 6,
  hs_false = 7,
  hs_alive = 8,
  hs_dead = 9,
  hs_committing_changes_in_tmp_doc = 10
};

enum class hs_dstate_apply_e {
  hs_ds_apply,
  hs_ds_unapply
};

enum class hs_doc_type_e {
  hs_doc_persistent,
  hs_doc_temporary
};

///@brief controls how begin_recording/record/end_recording works
enum class hs_doc_rec_type_e {
  hs_doc_rec_disabled,        /// disabled
  hs_doc_rec_as_new_epoch,    /// regular commit
  hs_doc_rec_init,            /// executes recursively from root
  hs_doc_rec_init_local,      /// executes !only! for current hs_doc
  hs_doc_rec_to_current_epoch /// record to current epoch
};

///Stores augment meta info for each child in each parent's epoch
struct hs_child_state_meta_t {
  epoch_t m_child_epoch{0};
  bool m_is_alive{false};
};

/**
* @brief The hist_doc_base_t class
*/
class hs_doc_base_t {

public:

  using epoch_t = size_t;

private:

  epoch_t p_cur_epoch{0};

  hs_doc_base_t *p_parent{nullptr};
  //hist_doc_base_t *p_super_parent{nullptr};
  std::vector<hs_doc_base_t*> p_children;
  std::vector<hs_doc_base_t*> p_just_added_children;
  std::map<epoch_t, std::map<hs_doc_base_t*, hs_child_state_meta_t>> p_children_states;
  std::map<hs_doc_base_t*, hs_child_state_meta_t> p_current_child_state;
  std::vector<epoch_t> p_hist_line{0};
  bool p_is_bad{false};
  bool p_ignore_changes{false};
  bool p_commit_exclusive_on_change{false};
  bool p_commit_exclusive_on_change_old{false};
  bool p_is_recording{false};
  bool p_auto_delete{false};
  bool p_auto_delete_children{false};
  hs_doc_rec_type_e p_cur_rec_type{hs_doc_rec_type_e::hs_doc_rec_disabled};
  hs_dstate_e p_dstate{hs_dstate_e::hs_dstate_inst};
  hs_doc_type_e p_doctype{hs_doc_type_e::hs_doc_persistent};

  /**
  * @brief get_children
  * @param idx
  * @return
  */
  hs_doc_base_t *get_child(size_t idx) const;

  /**
  * @brief is_children
  * @param child
  * @return
  */
  std::optional<size_t> is_child(hs_doc_base_t *child) const;

  /**
  * @brief get_children_count
  */
  size_t get_children_count() const;

protected:

  virtual hs_result_e reset_impl();
  virtual bool is_unmodified_impl();
  virtual hs_result_e squash_impl();
  virtual hs_result_e dstate_change(hs_dstate_apply_e ds_dir, epoch_t target);

  //recursive stuff
  void begin_recording_impl();
  void recording_impl();
  void end_recording_impl();

public:

  hs_doc_base_t ();
  virtual ~hs_doc_base_t();

  /**
   * @brief begin_recoring
   * @param init_as_base_commit
   */
  void begin_recording(hs_doc_rec_type_e record_type);
  virtual void on_begin_recording(){};
  virtual void on_recording(){};
  virtual void on_end_recording(){};
  void end_recording();
  bool get_is_recording();
  bool get_ignore_changes();
  void set_ignore_changes(bool new_ignore_changes);
  void clear_epoch_data();

  hs_doc_rec_type_e get_cur_rec_type() const;

  /**
  * @brief get_cur_epoch
  * @return
  */
  epoch_t get_cur_epoch() const;

  /**
  * @brief set_cur_epoch
  * @param cur_epoch
  * @return
  */
  hs_result_e set_cur_epoch(epoch_t cur_epoch, bool emit_event = true);

  /**
   * @brief on_epoch_changed
   * @param prev_epoch
   * @return
   */
  virtual hs_result_e on_epoch_changed(epoch_t prev_epoch);

  /**
   * @brief on_epoch_removed
   * @param target_epoch
   * @return
   */
  virtual hs_result_e on_epoch_removed(epoch_t target_epoch);

  /**
   * @brief commit_exclusive
   * @param child
   * @param child_epoch
   * @return
   */
  hs_result_e commit_exclusive(hs_doc_base_t *child = nullptr,
                               std::optional<epoch_t> child_epoch = std::nullopt,
                               bool triggers_dstate = true);

  /**
   * @brief on_commit_exclusive
   */
  virtual void on_commit_exclusive();

  /**
   * @brief get_commit_exclusive_on_change
   * @return
   */
  bool get_commit_exclusive_on_change();

  /**
   * @brief set_commit_exclusive_on_change
   * @param value
   */
  void set_commit_exclusive_on_change(bool value);

  /**
  * @brief set_delta_state_type - not implemented yet
  * @param new_dstate
  */
  void set_dstate_type(hs_dstate_e new_dstate);

  /**
   * @brief get_delta_state_type
   * @return
   */
  hs_dstate_e get_dstate_type();

  /**
   * @brief set_doctype
   * @param new_doctype
   */
  void set_doctype(hs_doc_type_e new_doctype);

  /**
   * @brief get_doctype
   * @return
   */
  hs_doc_type_e get_doctype();

  /**
   * @brief is_unmodified
   * @return
   */
  bool is_unmodified();

  /**
   * @brief reset current history document and all it's children
   * @return execution status
   */
  hs_result_e reset();

  /**
   * @brief squash
   * @param squashed_epochs
   * @return
   */
  hs_result_e squash();

  /**
  * @brief push_epoch insert new epoch at cursor
  * @param new_epoch
  * @return
  */
  std::tuple<hs_result_e, std::optional<epoch_t>> push_epoch(
      std::optional<epoch_t> new_epoch_ex = std::nullopt,
      bool checkout_to_new_epoch = false,
      bool triggers_dstate = false);

  /**
  * @brief get_history_size
  */
  size_t get_history_size() const;

  /**
   * @brief get_history
   * @return vector of epochs
   */
  std::vector<epoch_t> get_history() const;

  /**
  * @brief augment_epoch
  * @param child pointer to child
  * @param child_epoch
  * @param target_epoch
  */
  hs_result_e augment_epoch(epoch_t target_epoch, hs_doc_base_t* child, epoch_t child_epoch,
                            bool alive = true);

  /**
  * @brief get_augmented_count
  * @param target_epoch
  * @return
  */
  size_t get_augmented_count(epoch_t target_epoch) const;

  /**
   * @brief is_augmented_by
   * @param target_epoch
   * @param child
   * @return
   */
  bool is_augmented_by(epoch_t target_epoch, hs_doc_base_t *child);

  /**
  * @brief remove_augment_from_epoch
  * @param child
  * @param child_epoch
  * @param target_epoch
  * @return
  */
  hs_result_e remove_augment_from_epoch(hs_doc_base_t* child, epoch_t target_epoch);

  /**
   * @brief is_child_alive
   * @param target_epoch
   * @param child
   * @return
   */
  hs_result_e is_child_alive(epoch_t target_epoch, hs_doc_base_t* child) const;

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
  hs_result_e checkout_to_epoch(epoch_t target_epoch,
                                bool process_dstates = true);

  /**
   * @brief checkout_by_dist
   * @param dist
   * @return
   */
  hs_result_e checkout_by_dist(int dist);

  /**
   * @brief can_checkout_by_dist
   * @param dist
   * @return
   */
  bool can_checkout_by_dist(int dist);

  /**
  * @brief add_child
  * @param child
  * @return
  */
  hs_result_e add_hs_child(hs_doc_base_t *child, bool add_new_epoch = false);

  /**
   * @brief delete_hs_child
   * @param child
   * @return
   */
  hs_result_e set_alive_hs_child(hs_doc_base_t *child, bool alive = true);

  /**
   * @brief get_hs_children_count
   * @return
   */
  size_t get_hs_children_count();

  /**
   * @brief get_hs_child
   * @param child_idx
   * @return
   */
  hs_doc_base_t *get_hs_child(size_t child_idx);

  /**
  * @brief get_root
  * @return
  */
  hs_doc_base_t *get_parent();

  /**
   * @brief get_super_root
   * @return
   */
  hs_doc_base_t *get_super_parent();

  bool get_auto_delete() const ;
  void set_auto_delete(bool value);

  bool get_auto_delete_children() const ;
  void set_auto_delete_children(bool value);

  bool is_child_unused(hs_doc_base_t *child);

  /**
  * @brief remove_child removes child completely by id
  * @param child_id
  */
  hs_result_e hs_remove_child(size_t child_id);

  /**
  * @brief remove_child removes child completely by pointer
  * @param child
  */
  hs_result_e hs_remove_child(hs_doc_base_t *child, bool is_child_deletion_requested = false);

  virtual void request_child_deletion(hs_doc_base_t *child);


};

template<typename STYPE>
class hs_doc_t : public hs_doc_base_t {

private:

  STYPE p_cur_value{};
  std::map<epoch_t, STYPE> p_stored_values;

protected:

  hs_result_e reset_impl() override {

    if (p_stored_values.empty())
      return hs_result_e::hs_invalid_epoch;

    auto stored_val_it = p_stored_values.find(get_cur_epoch());
    if (stored_val_it == end(p_stored_values))
      return hs_result_e::hs_invalid_epoch;

    p_cur_value = stored_val_it->second;
    return hs_result_e::hs_success;

  }

  void on_recording() override {

    if (get_cur_rec_type() == hs_doc_rec_type_e::hs_doc_rec_init) {
      p_stored_values.clear();
    }

    p_stored_values[get_cur_epoch()] = p_cur_value;

  }

  hs_result_e squash_impl() override {
    p_stored_values.clear();
    p_stored_values[0] = p_cur_value;
    return hs_result_e::hs_success;
  }

public:

  hs_doc_t(STYPE &&new_val) {
    p_stored_values[get_cur_epoch()] = new_val;
    p_cur_value = p_stored_values[get_cur_epoch()];
  }

  hs_doc_t() {
    p_stored_values[get_cur_epoch()] = p_cur_value;
  }

  hs_result_e push_epoch_with_value(STYPE &&new_val,
                                    std::optional<epoch_t> new_epoch = std::nullopt,
                                    bool checkout_to_new_epoch = false) {

    auto push_epoch_res = push_epoch(new_epoch, checkout_to_new_epoch);
    if (std::get<0>(push_epoch_res) == hs_result_e::hs_success) {
      p_stored_values[*std::get<1>(push_epoch_res)] = new_val;
      return hs_result_e::hs_success;
    } else {
      return hs_result_e::hs_error;
    }

  }

  void on_commit_exclusive() override {
    p_stored_values[get_cur_epoch()] = p_cur_value;
  }

  hs_result_e commit_value_exclusive(STYPE &&new_val,
                                     std::optional<epoch_t> new_epoch = std::nullopt) {

    if (get_doctype() != hs_doc_type_e::hs_doc_persistent)
      return hs_result_e::hs_committing_changes_in_tmp_doc;

    STYPE loc_var(new_val);
    auto push_epoch_with_value_res =
        push_epoch_with_value(std::move(loc_var), new_epoch, true);

    if (push_epoch_with_value_res != hs_result_e::hs_success)
      return hs_result_e::hs_error;

    commit_exclusive();
    return hs_result_e::hs_success;

  }

  hs_result_e on_epoch_changed(epoch_t prev_epoch) override {
    auto cur_epoch = get_cur_epoch();
    auto val_it = p_stored_values.find(cur_epoch);
    if (val_it != p_stored_values.end()) {
      p_cur_value = val_it->second;
      return hs_result_e::hs_success;
    } else {
      return hs_result_e::hs_error;
    }
  }

  hs_result_e on_epoch_removed(epoch_t epoch_to_remove) override {
    auto it = p_stored_values.find(epoch_to_remove);
    if (it != p_stored_values.end()) {
      p_stored_values.erase(it);
      return hs_result_e::hs_success;
    } else {
      return hs_result_e::hs_error;
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

    if constexpr (std::is_floating_point<STYPE>::value) {
      if (std::fabs(p_cur_value - new_val) < std::numeric_limits<STYPE>::epsilon())
        return;
    } else {
      if (p_cur_value == new_val)
        return;
    }

    p_cur_value = new_val;

    if (get_ignore_changes())
      return;

    if (hs_doc_base_t *super_parent = get_super_parent();
        get_doctype() == hs_doc_type_e::hs_doc_persistent
        && super_parent
        && super_parent->get_commit_exclusive_on_change())
      commit_value_exclusive(STYPE(p_cur_value));

  }

  void set_cvalue(STYPE new_val) {
    set_value(std::move(new_val));
  }

};

template<typename STYPE>
class hist_property_t : public hs_doc_t<STYPE> {

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
