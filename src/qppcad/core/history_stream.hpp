#ifndef QPPCAD_HISTORY_STREAM
#define QPPCAD_HISTORY_STREAM

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    template<typename STORED_TYPE>
    class history_stream_t {

    private:

      std::map<size_t, STORED_TYPE> p_value_stream{{0, STORED_TYPE()}};
      std::vector<size_t> p_history_line{0};
      size_t p_cur_history_epoch{0};
      size_t p_max_history_epoch{0};

    public:

      void advance_epoch(STORED_TYPE &&new_value, std::optional<size_t> new_epoch = std::nullopt) {

        // provide epoch index or do incremental epoch
        size_t new_epoch_ex =
            (new_epoch && *new_epoch > p_max_history_epoch ? *new_epoch : p_max_history_epoch + 1);
        p_value_stream.insert({new_epoch_ex, std::forward<STORED_TYPE>(new_value)});
        p_history_line.push_back(new_epoch_ex);
        p_max_history_epoch = new_epoch_ex;
        p_cur_history_epoch = new_epoch_ex;

      }

      size_t get_cur_epoch() {

        return p_cur_history_epoch;

      }

    };

  }

}

#endif
