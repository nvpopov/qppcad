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
      hr_invalid_child_epoch = 5
    };

    /**
     * @brief The hist_doc_base_t class
     */
    class hist_doc_base_t {

    public:

      using epoch_t = std::size_t;
      using self_t = hist_doc_base_t;

    private:

      epoch_t p_cur_epoch{0};
      self_t *p_parent{nullptr};
      std::vector<self_t*> p_childs;
      std::map<epoch_t, std::vector<std::tuple<self_t*, epoch_t>>> p_childs_states;
      std::set<epoch_t> p_hist_line{0};
      bool p_is_bad{false};
      bool p_is_dirty{false};
      hist_doc_delta_state_e p_dstate{hist_doc_delta_state_e::delta_instant};

    public:

      /**
       * @brief get_cur_epoch
       * @return
       */
      epoch_t get_cur_epoch() { return p_cur_epoch; }

      /**
       * @brief set_cur_epoch
       * @param cur_epoch
       * @return
       */
      hr_result_e set_cur_epoch(epoch_t cur_epoch) {

        auto it_ce = std::find(std::begin(p_hist_line), std::end(p_hist_line), cur_epoch);
        if (it_ce != std::end(p_hist_line)) {
            epoch_t prev_epoch = p_cur_epoch;
            p_cur_epoch = cur_epoch;
            return on_epoch_changed(prev_epoch);
          }

        return hr_result_e::hr_invalid_epoch;

      }

      virtual hr_result_e on_epoch_changed(epoch_t prev_epoch) {
        return hr_result_e::hr_success;
      }

      /**
       * @brief get_delta_state_type - not implemented yet
       * @return
       */
      hist_doc_delta_state_e get_delta_state_type() { return p_dstate; }

      /**
       * @brief set_delta_state_type - not implemented yet
       * @param new_dstate
       */
      void set_delta_state_type(hist_doc_delta_state_e new_dstate) { p_dstate = new_dstate;}

      /**
       * @brief is_dirty - not implemented yet
       * @return
       */
      bool is_dirty() { return p_is_dirty; }

      /**
       * @brief mark_as_dirty - not implemented yet
       */
      void mark_as_dirty() { p_is_dirty = true; }

      /**
       * @brief push_epoch
       * @param new_epoch
       * @return
       */
      hr_result_e push_epoch(std::optional<epoch_t> new_epoch_ex = std::nullopt) {

        epoch_t new_epoch = (new_epoch_ex) ?
                            *new_epoch_ex :
                            *std::max_element(p_hist_line.begin(), p_hist_line.end()) + 1;

        if (p_hist_line.empty()) {
            p_hist_line.insert(new_epoch);
            return hr_result_e::hr_success;
          }

        auto last = p_hist_line.end();
        if (new_epoch > *last) {
            p_is_bad = true;
            return hr_result_e::hr_invalid_epoch;
          } else {
            p_hist_line.insert(new_epoch);
            return hr_result_e::hr_success;
          }

      }

      /**
       * @brief get_history_size
       */
      auto get_history_size() {return p_hist_line.size();}

      /**
       * @brief augment_epoch
       * @param child pointer to child
       * @param child_epoch
       * @param target_epoch
       */
      hr_result_e augment_epoch(epoch_t target_epoch, self_t* child, epoch_t child_epoch) {

        if (!child) {
            return hr_result_e::hr_invalid_child;
        }

        if (p_hist_line.find(target_epoch) == std::end(p_hist_line)) {
            return hr_result_e::hr_invalid_epoch;
          }

        if (child->p_hist_line.find(child_epoch) == std::end(child->p_hist_line)) {
            return hr_result_e::hr_invalid_child_epoch;
          }

        auto &aug_elist = p_childs_states[target_epoch];
        auto find_fnc = [child](std::tuple<self_t*, epoch_t> &elem) {
          return std::get<0>(elem) == child;
        };

        auto it1 = std::find_if(std::begin(aug_elist), std::end(aug_elist), find_fnc);
        if (it1 != std::end(aug_elist)) {
            aug_elist[std::distance(std::begin(aug_elist), it1)] = {child, child_epoch};
          } else {
            aug_elist.push_back({child, child_epoch});
          }

        return hr_result_e::hr_success;

      }

      /**
       * @brief get_augmented_count
       * @param target_epoch
       * @return
       */
      size_t get_augmented_count(epoch_t target_epoch) {
        auto &aug_elist = p_childs_states[target_epoch];
        return aug_elist.size();
      }

      /**
       * @brief has_epoch
       * @param target_epoch
       * @return
       */
      bool has_epoch(epoch_t target_epoch) {
        return p_hist_line.find(target_epoch) != std::end(p_hist_line);
      }

      /**
       * @brief remove_augment_from_epoch
       * @param child
       * @param child_epoch
       * @param target_epoch
       * @return
       */
      hr_result_e remove_augment_from_epoch(self_t* child, epoch_t child_epoch,
                                            epoch_t target_epoch) {

        auto epoch_it = p_childs_states.find(target_epoch);
        if (epoch_it == std::end(p_childs_states)) return hr_result_e::hr_invalid_epoch;
        auto find_fnc = [child, child_epoch](std::tuple<self_t*, epoch_t> &elem) {
          return std::get<0>(elem) == child && std::get<1>(elem) == child_epoch;
        };
        auto ch_epoch_it = std::find_if(std::begin(epoch_it->second),
                                        std::end(epoch_it->second),
                                        find_fnc);
        if (ch_epoch_it == std::end(epoch_it->second)) return hr_result_e::hr_invalid_epoch;
        epoch_it->second.erase(ch_epoch_it);
        return hr_result_e::hr_success;

      }

      /**
       * @brief checkout_to_epoch
       * @param target_epoch
       * @return
       */
      hr_result_e checkout_to_epoch(epoch_t target_epoch) {

        if (p_cur_epoch == target_epoch) {
            return hr_result_e::hr_success;
          }

        hr_result_e cur_res = set_cur_epoch(target_epoch);
        if (cur_res != hr_result_e::hr_success) {
            return hr_result_e::hr_invalid_epoch;
          }

        epoch_t cur_epoch = get_cur_epoch();
        auto epoch_it = p_childs_states.find(cur_epoch);
        if (epoch_it != std::end(p_childs_states)) {

            auto &epoch_aug_vec = epoch_it->second;

            //check that augmented data is valid
            size_t valid_childs{0};
            for (auto &elem : epoch_aug_vec) {

                auto child = std::get<0>(elem);
                auto child_epoch = std::get<1>(elem);

                if (child && child->has_epoch(child_epoch)) {
                    valid_childs++;
                  } else {
                    return hr_result_e::hr_invalid_epoch;
                  }

              }

            if (valid_childs != epoch_aug_vec.size()) {
                return hr_result_e::hr_invalid_epoch;
              }

            for (auto &elem : epoch_aug_vec) {
                auto child = std::get<0>(elem);
                auto child_epoch = std::get<1>(elem);
                child->checkout_to_epoch(child_epoch);
              }

          }

        return hr_result_e::hr_success;

      }

      /**
       * @brief add_child
       * @param child
       */
      void add_child(self_t *child) {
        if (child) {
            child->p_parent = this;
            p_childs.push_back(child);
            augment_epoch(get_cur_epoch(), child, child->get_cur_epoch());
          }
      }

      /**
       * @brief get_root
       * @return
       */
      self_t *get_root() {
        if (p_parent)
          return p_parent->get_root();
        return this;
      }

      /**
       * @brief get_children
       * @param idx
       * @return
       */
      self_t *get_children(size_t idx) {
        if (idx < p_childs.size())
          return p_childs[idx];
        return nullptr;
      }

      /**
       * @brief is_children
       * @param child
       * @return
       */
      std::optional<size_t> is_children(self_t *child) {
        if (!child) return std::nullopt;
        auto it1 = std::find(std::begin(p_childs), std::end(p_childs), child);
        return (it1 != std::end(p_childs)
                ? std::optional<size_t>{std::distance(std::begin(p_childs), it1)} : std::nullopt);
      }

      /**
       * @brief remove_child
       * @param child_id
       */
      void remove_child(size_t child_id) {
        if (child_id < p_childs.size())
          p_childs.erase(std::begin(p_childs) + child_id);
      }

      /**
       * @brief remove_child
       * @param child
       */
      void remove_child(self_t *child) {
        auto it1 = std::find(std::begin(p_childs), std::end(p_childs), child);
        if (it1 != std::end(p_childs)) p_childs.erase(it1);
      }

      /**
       * @brief get_children_count
       */
      auto get_children_count() { return p_childs.size(); }
      //end of children stuff

    };

    template<typename STORED_TYPE>
    class hist_doc_t : public hist_doc_base_t {

    private:

      STORED_TYPE p_cur_value;
      std::map<epoch_t, STORED_TYPE> p_stored_values;

    public:

      void push_epoch_ex(STORED_TYPE &&new_val, std::optional<epoch_t> new_epoch = std::nullopt) {

      }

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
