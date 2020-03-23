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

    public:

      epoch_t get_cur_epoch() { return p_cur_epoch; }
      bool set_cur_epoch(epoch_t cur_epoch) {

        auto it_ce = std::find(std::begin(p_history_line), std::end(p_history_line), cur_epoch);
        if (it_ce != std::end(p_history_line)) {
            p_cur_epoch = cur_epoch;
            return true;
          }

        return false;

      }

      bool push_epoch(epoch_t new_epoch) {

        if (p_history_line.empty()) {
            p_history_line.insert(new_epoch);
            return true;
          }

        auto last = p_history_line.end();
        if (new_epoch <= *last) {
            p_is_bad = true;
            return false;
          } else {
            p_history_line.insert(new_epoch);
            return true;
          }

      }

      auto get_history_size() {return p_history_line.size();}

      void augment_epoch(self_t* child, epoch_t epoch) {

      }

      void remove_augment_from_epoch(self_t* child, epoch_t epoch) {

      }

      void checkout_to_epoch(epoch_t target_epoch) {

      }

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
