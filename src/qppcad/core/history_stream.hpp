#ifndef QPPCAD_HISTORY_STREAM
#define QPPCAD_HISTORY_STREAM

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    class history_document_base_t;
    using epoch_t = std::size_t;

    enum history_document_delta_state_e {
      delta_instant,
      delta_incremental
    };

    enum history_result_e {
      hr_error = 0,
      hr_success = 1,
      hr_invalid_epoch = 2,
    };

    class history_document_base_t {

    public:

      using epoch_t = std::size_t;
      using self_t = history_document_base_t;

    private:

      epoch_t p_cur_epoch{0};
      std::vector<self_t*> p_childs;
      std::map<epoch_t, std::vector<std::tuple<self_t*, epoch_t>>> p_childs_states;
      std::set<epoch_t> p_history_line{0};
      bool p_is_bad{false};
      bool p_is_dirty{false};

    public:

      epoch_t get_cur_epoch() { return p_cur_epoch; }
      history_result_e set_cur_epoch(epoch_t cur_epoch) {

        auto it_ce = std::find(std::begin(p_history_line), std::end(p_history_line), cur_epoch);
        if (it_ce != std::end(p_history_line)) {
            p_cur_epoch = cur_epoch;
            return history_result_e::hr_success;
          }

        return history_result_e::hr_invalid_epoch;

      }

      history_result_e push_epoch(epoch_t new_epoch) {

        if (p_history_line.empty()) {
            p_history_line.insert(new_epoch);
            return history_result_e::hr_success;
          }

        auto last = p_history_line.end();
        if (new_epoch > *last) {
            p_is_bad = true;
            return history_result_e::hr_invalid_epoch;
          } else {
            p_history_line.insert(new_epoch);
            return history_result_e::hr_success;
          }

      }

      auto get_history_size() {return p_history_line.size();}

      void augment_epoch(self_t* child, epoch_t epoch) {

      }

      void remove_augment_from_epoch(self_t* child, epoch_t epoch) {

      }

      void checkout_to_epoch(epoch_t target_epoch) {

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
