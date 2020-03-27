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
      std::set<epoch_t> p_history_line{0};
      bool p_is_bad{false};
      bool p_is_dirty{false};
      hist_doc_delta_state_e p_dstate{hist_doc_delta_state_e::delta_instant};

    public:

      epoch_t get_cur_epoch() { return p_cur_epoch; }
      hr_result_e set_cur_epoch(epoch_t cur_epoch) {

        auto it_ce = std::find(std::begin(p_history_line), std::end(p_history_line), cur_epoch);
        if (it_ce != std::end(p_history_line)) {
            p_cur_epoch = cur_epoch;
            return hr_result_e::hr_success;
          }

        return hr_result_e::hr_invalid_epoch;

      }

      hist_doc_delta_state_e get_delta_state_type() { return p_dstate; }
      void set_delta_state_type(hist_doc_delta_state_e new_dstate) { p_dstate = new_dstate;}

      hr_result_e push_epoch(epoch_t new_epoch) {

        if (p_history_line.empty()) {
            p_history_line.insert(new_epoch);
            return hr_result_e::hr_success;
          }

        auto last = p_history_line.end();
        if (new_epoch > *last) {
            p_is_bad = true;
            return hr_result_e::hr_invalid_epoch;
          } else {
            p_history_line.insert(new_epoch);
            return hr_result_e::hr_success;
          }

      }

      auto get_history_size() {return p_history_line.size();}

      void augment_epoch(self_t* child, epoch_t child_epoch, epoch_t target_epoch) {

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

      }

      size_t get_augmented_count(epoch_t target_epoch) {
        auto &aug_elist = p_childs_states[target_epoch];
        return aug_elist.size();
      }

      bool has_epoch(epoch_t target_epoch) {
        return p_history_line.find(target_epoch) != std::end(p_history_line);
      }

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

      //children stuff
      void add_child(self_t *child) {
        if (child) p_childs.push_back(child);
      }

      self_t *get_children(size_t idx) {
        if (idx < p_childs.size())
          return p_childs[idx];
        return nullptr;
      }

      std::optional<size_t> is_children(self_t *child) {
        if (!child) return std::nullopt;
        auto it1 = std::find(std::begin(p_childs), std::end(p_childs), child);
        return (it1 != std::end(p_childs)
                ? std::optional<size_t>{std::distance(std::begin(p_childs), it1)} : std::nullopt);
      }

      void remove_child(size_t child_id) {
        if (child_id < p_childs.size())
          p_childs.erase(std::begin(p_childs) + child_id);
      }

      void remove_child(self_t *child) {
        auto it1 = std::find(std::begin(p_childs), std::end(p_childs), child);
        if (it1 != std::end(p_childs)) p_childs.erase(it1);
      }

      auto get_children_count() { return p_childs.size(); }
      //end of children stuff

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
