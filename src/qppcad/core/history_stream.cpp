#include <qppcad/core/history_stream.hpp>

namespace qpp {

  namespace cad {

    hist_doc_base_t::hist_doc_base_t() {

    }

    hist_doc_base_t::~hist_doc_base_t() {

    }

    hist_doc_base_t::epoch_t hist_doc_base_t::get_cur_epoch() {
      return p_cur_epoch;
    }

    hr_result_e hist_doc_base_t::set_cur_epoch(hist_doc_base_t::epoch_t cur_epoch) {

      auto it_ce = std::find(std::begin(p_hist_line), std::end(p_hist_line), cur_epoch);
      if (it_ce != std::end(p_hist_line)) {
          epoch_t prev_epoch = p_cur_epoch;
          p_cur_epoch = cur_epoch;
          return on_epoch_changed(prev_epoch);
        }

      return hr_result_e::hr_invalid_epoch;

    }

    hr_result_e hist_doc_base_t::on_epoch_changed(hist_doc_base_t::epoch_t prev_epoch) {
      return hr_result_e::hr_success;
    }

    hist_doc_delta_state_e hist_doc_base_t::get_delta_state_type() {
      return p_dstate;
    }

    void hist_doc_base_t::set_delta_state_type(hist_doc_delta_state_e new_dstate) {
      p_dstate = new_dstate;
    }

    bool hist_doc_base_t::is_dirty() {
      return p_is_dirty;
    }

    void hist_doc_base_t::mark_as_dirty() {
      p_is_dirty = true;
    }

    std::tuple<hr_result_e, std::optional<hist_doc_base_t::epoch_t> > hist_doc_base_t::push_epoch(
        std::optional<hist_doc_base_t::epoch_t> new_epoch_ex) {

      epoch_t new_epoch = (new_epoch_ex) ?
            *new_epoch_ex :
            *std::max_element(p_hist_line.begin(), p_hist_line.end()) + 1;

      if (p_hist_line.empty()) {
          p_hist_line.insert(new_epoch);
          return {hr_result_e::hr_success, new_epoch};
        }

      auto last = p_hist_line.end();
      if (new_epoch > *last) {
          p_is_bad = true;
          return {hr_result_e::hr_invalid_epoch, std::nullopt};
        } else {
          p_hist_line.insert(new_epoch);
          return {hr_result_e::hr_success, new_epoch};
        }

    }

    size_t hist_doc_base_t::get_history_size() {
      return p_hist_line.size();
    }

    hr_result_e hist_doc_base_t::augment_epoch(hist_doc_base_t::epoch_t target_epoch,
                                               hist_doc_base_t::self_t *child,
                                               hist_doc_base_t::epoch_t child_epoch) {

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

    size_t hist_doc_base_t::get_augmented_count(hist_doc_base_t::epoch_t target_epoch) {
      auto &aug_elist = p_childs_states[target_epoch];
      return aug_elist.size();
    }

    bool hist_doc_base_t::has_epoch(hist_doc_base_t::epoch_t target_epoch) {
      return p_hist_line.find(target_epoch) != std::end(p_hist_line);
    }

    hr_result_e hist_doc_base_t::remove_augment_from_epoch(hist_doc_base_t::self_t *child,
                                                           hist_doc_base_t::epoch_t child_epoch,
                                                           hist_doc_base_t::epoch_t target_epoch) {

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

    hr_result_e hist_doc_base_t::checkout_to_epoch(hist_doc_base_t::epoch_t target_epoch) {

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

    void hist_doc_base_t::add_child(hist_doc_base_t::self_t *child) {
      if (child) {
          child->p_parent = this;
          p_childs.push_back(child);
          augment_epoch(get_cur_epoch(), child, child->get_cur_epoch());
        }
    }

    hist_doc_base_t::self_t *hist_doc_base_t::get_root() {
      if (p_parent)
        return p_parent->get_root();
      return this;
    }

    hist_doc_base_t::self_t *hist_doc_base_t::get_children(size_t idx) {
      if (idx < p_childs.size())
        return p_childs[idx];
      return nullptr;
    }

    std::optional<size_t> hist_doc_base_t::is_children(hist_doc_base_t::self_t *child) {
      if (!child) return std::nullopt;
      auto it1 = std::find(std::begin(p_childs), std::end(p_childs), child);
      return (it1 != std::end(p_childs)
          ? std::optional<size_t>{std::distance(std::begin(p_childs), it1)} : std::nullopt);
    }

    void hist_doc_base_t::remove_child(size_t child_id) {
      if (child_id < p_childs.size())
        p_childs.erase(std::begin(p_childs) + child_id);
    }

    void hist_doc_base_t::remove_child(hist_doc_base_t::self_t *child) {
      auto it1 = std::find(std::begin(p_childs), std::end(p_childs), child);
      if (it1 != std::end(p_childs)) p_childs.erase(it1);
    }

    size_t hist_doc_base_t::get_children_count() { return p_childs.size(); }

  } // namespace qpp::cad

} // namespace qpp
