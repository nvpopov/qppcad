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

      void push_epoch(STORED_TYPE &&new_value, std::optional<size_t> new_epoch = std::nullopt) {

        // do we insert at end of current history line?
        // if not - truncate history line
        if (p_cur_history_epoch != p_max_history_epoch) {
            auto it_cur = std::find(std::begin(p_history_line),
                                    std::end(p_history_line),
                                    p_cur_history_epoch);
            if (it_cur != p_history_line.end())
              p_history_line.erase(++it_cur, std::end(p_history_line));
          }

        // provide epoch index or do incremental epoch
        size_t new_epoch_ex =
            (new_epoch && *new_epoch > p_max_history_epoch ? *new_epoch : p_max_history_epoch + 1);
        p_value_stream.insert({new_epoch_ex, std::forward<STORED_TYPE>(new_value)});
        p_history_line.push_back(new_epoch_ex);
        p_max_history_epoch = new_epoch_ex;
        p_cur_history_epoch = new_epoch_ex;

      }

      bool epoch_exists_in_current_history(size_t epoch_id) {
        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), epoch_id);
        return it1 != p_history_line.end();
      }

      size_t get_cur_epoch() {
        return p_cur_history_epoch;
      }

      void set_cur_epoch(size_t epoch_id) {
        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), epoch_id);
        if (it1 != p_history_line.end()) p_cur_history_epoch = epoch_id;
      }

      std::optional<STORED_TYPE> get_epoch_value(size_t epoch_id) {
        auto it1 = p_value_stream.find(epoch_id);
        return it1 != p_value_stream.end() ?
               std::optional<STORED_TYPE>(it1.second) : std::nullopt;
      }

      size_t get_cur_history_line_size() {
        return p_history_line.size();
      }

      void print_debug() {

        std::cout << std::endl << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;

        for (size_t i = 0; i < p_history_line.size(); i++)
          std::cout << "Epoch id = "
                    << p_history_line[i]
                    << ", value = "
                    << p_value_stream[p_history_line[i]]
                    << std::endl;


        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

      }

      history_stream_t() {

      }

      history_stream_t(STORED_TYPE&& default_value) {
        p_value_stream[0] = default_value;
      }

    };

  }

}

#endif
