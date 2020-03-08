#ifndef QPPCAD_HISTORY_STREAM
#define QPPCAD_HISTORY_STREAM

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    class history_stream_base_t;

    /*
     * history_stream_base_t
     */
    class history_stream_base_t {

    private:

      history_stream_base_t *p_top_level_hs{nullptr};
      std::vector<history_stream_base_t*> p_hist_lines;

      std::vector<size_t> p_history_line{0};
      size_t p_cur_history_epoch{0};
      size_t p_max_history_epoch{0};

    public:

      virtual void advance_to_epoch(size_t epoch_id) {
        for (auto elem : p_hist_lines)
          if (elem) elem->advance_to_epoch(epoch_id);
      }

      virtual ~history_stream_base_t() = default;

      void set_top_level_hs(history_stream_base_t *top_level_hs){p_top_level_hs = top_level_hs;}
      history_stream_base_t *get_top_level_hs(){return p_top_level_hs;}

      size_t get_cur_epoch() {
        return p_cur_history_epoch;
      }

      void set_cur_epoch(size_t new_epoch) {
        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), new_epoch);
        if (it1 != p_history_line.end()) p_cur_history_epoch = new_epoch;
      }

      size_t get_max_epoch() {
        return p_max_history_epoch;
      }

      void set_max_epoch(size_t new_epoch) {
        p_max_history_epoch = new_epoch;
      }

      bool epoch_exists_in_current_history(size_t epoch_id) {
        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), epoch_id);
        return it1 != p_history_line.end();
      }

      size_t get_hist_size() {
        return p_history_line.size();
      }

      size_t get_hist_at(size_t at_step) {
        return p_history_line.at(at_step);
      }

      //return new epoch id on success
      std::optional<size_t> push_epoch(std::optional<size_t> new_epoch = std::nullopt) {

        if (p_cur_history_epoch != p_max_history_epoch) {
            auto it_cur = std::find(std::begin(p_history_line),
                                    std::end(p_history_line),
                                    p_cur_history_epoch);

            std::advance(it_cur, 1); //advance to next element

            if (it_cur != p_history_line.end()) {
                //or (auto it_i = it_cur; it_i != std::end(p_history_line); it_i++)
                //p_value_stream.erase(*it_i);
                p_history_line.erase(it_cur, std::end(p_history_line));
              }

          }

        size_t new_epoch_ex =
            (new_epoch && *new_epoch > p_max_history_epoch ? *new_epoch : p_max_history_epoch + 1);

        p_history_line.push_back(new_epoch_ex);
        p_max_history_epoch = new_epoch_ex;
        p_cur_history_epoch = new_epoch_ex;

        return std::optional<size_t>(new_epoch_ex);

      }

    };

    /*
     * history_stream_t<STORED_TYPE>
     */
    template<typename STORED_TYPE>
    class history_stream_t : public history_stream_base_t {

    private:
      friend history_stream_base_t;
      std::map<size_t, STORED_TYPE> p_value_stream{{0, STORED_TYPE()}};

    public:

      void push_epoch_with_data(STORED_TYPE &&new_value,
                               std::optional<size_t> new_epoch = std::nullopt) {

        auto new_epoch_opt = push_epoch(new_epoch);
        if (new_epoch_opt)
          p_value_stream.insert({*new_epoch_opt, std::forward<STORED_TYPE>(new_value)});

      }

      std::optional<STORED_TYPE> get_epoch_value(size_t epoch_id) {
        auto it1 = p_value_stream.find(epoch_id);
        return it1 != p_value_stream.end() ?
              std::optional<STORED_TYPE>(it1.second) : std::nullopt;
      }

      void print_debug() {

        std::cout << std::endl << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
        std::cout << "Stored data:" << std::endl;

        for (auto elem : p_value_stream)
          std::cout << "Epoch id = "
                    << elem.first
                    << ", value = "
                    << elem.second
                    << std::endl;

        std::cout << "History line:" << std::endl;

        for (size_t i = 0; i < get_hist_size(); i++)
          std::cout << "Epoch id = "
                    << get_hist_at(i)
                       << ", value = "
                       << p_value_stream[get_hist_at(i)]
                       << std::endl;


        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

      }

      void advance_to_epoch(size_t epoch_id) override {
        set_cur_epoch(epoch_id);
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
