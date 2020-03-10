#ifndef QPPCAD_HISTORY_STREAM
#define QPPCAD_HISTORY_STREAM

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    /**********************************************************************************************
     *
     * history_stream_base_t
     *
     *********************************************************************************************/
    template<typename EPOCH_TYPE = size_t>
    class history_stream_base_t {

    public:

      using history_line_t = std::vector<EPOCH_TYPE>;
      using ref_history_line_t = std::reference_wrapper<history_line_t>;

    private:

      history_stream_base_t<EPOCH_TYPE> *p_parent{nullptr};
      std::vector<history_stream_base_t<EPOCH_TYPE>*> p_childs;
      history_line_t p_history_line{0};
      EPOCH_TYPE p_cur_history_epoch{0};
      EPOCH_TYPE p_max_history_epoch{0};

    public:

      virtual ~history_stream_base_t() = default;

      EPOCH_TYPE get_cur_epoch() {
        return p_cur_history_epoch;
      }

      void set_parent(history_stream_base_t<EPOCH_TYPE> *parent){
        p_parent = parent;
        if (p_parent) p_parent->on_new_child_added(this);
      }

      history_stream_base_t<EPOCH_TYPE> *get_parent() {
        return p_parent;
      }

      history_stream_base_t<EPOCH_TYPE>* get_root() {

        if (!p_parent) return this;
        history_stream_base_t<EPOCH_TYPE> *cur_elem = this;
        while (cur_elem->p_parent != nullptr) {
            cur_elem = cur_elem->p_parent;
          }
        return cur_elem;

      }

      void on_new_child_added(history_stream_base_t<EPOCH_TYPE> *new_child) {
        if (new_child) {
            p_childs.push_back(new_child);
            p_max_history_epoch = get_max_epoch_recursive();
          }
      }

      EPOCH_TYPE get_max_epoch() {
        return p_max_history_epoch;
      }

      void set_max_epoch(EPOCH_TYPE new_epoch) {
        p_max_history_epoch = new_epoch;
      }

      EPOCH_TYPE get_max_epoch_recursive() {

        std::set<EPOCH_TYPE> max_epochs;
        max_epochs.insert(p_max_history_epoch);

        for (auto child : p_childs)
          if (child) max_epochs.insert(child->get_max_epoch_recursive());

        return max_epochs.empty() ? 0 : *max_epochs.end();

      }

      void checkout_to_epoch(EPOCH_TYPE epoch_id) {

        auto cur_root = get_root();
        if (!cur_root) {
            //TODO: error
            return;
          }
        cur_root->checkout_to_epoch_propagate(epoch_id);

      }

      void checkout_to_epoch_propagate(EPOCH_TYPE epoch_id) {

        set_cur_epoch_failsafe(epoch_id);
        for (auto child : p_childs)
          if (child) checkout_to_epoch_propagate(epoch_id);

      }

      void set_cur_epoch(EPOCH_TYPE new_epoch) {
        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), new_epoch);
        if (it1 != p_history_line.end()) p_cur_history_epoch = new_epoch;
      }

      void set_cur_epoch_failsafe(EPOCH_TYPE new_epoch) {

        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), new_epoch);

        if (it1 != p_history_line.end()) {
            p_cur_history_epoch = new_epoch;
            return;
          }

        //find min diff epoch
        EPOCH_TYPE cur_failsafe = 0;
        for (auto &elem : p_history_line)
          if (elem < new_epoch) {
              cur_failsafe = elem;
              break;
            }

        p_cur_history_epoch = cur_failsafe;

      }

      bool epoch_exists_in_current_history(EPOCH_TYPE epoch_id) {
        auto it1 = std::find(std::begin(p_history_line), std::end(p_history_line), epoch_id);
        return it1 != p_history_line.end();
      }

      size_t get_hist_size() {
        return p_history_line.size();
      }

      EPOCH_TYPE get_hist_at(size_t at_step) {
        return p_history_line.at(at_step);
      }

      void propagate_max_epoch(EPOCH_TYPE new_max_epoch, bool first = true) {
        if (!first) p_max_history_epoch = new_max_epoch;
        if (p_parent) p_parent->propagate_max_epoch(new_max_epoch, false);
      }

      //return new epoch id on success
      std::optional<EPOCH_TYPE> push_epoch(
          std::optional<EPOCH_TYPE> new_epoch = std::nullopt,
          std::optional<ref_history_line_t> epoch_to_delete = std::nullopt) {

        if (p_cur_history_epoch != p_max_history_epoch) {
            auto it_cur = std::find(std::begin(p_history_line),
                                    std::end(p_history_line),
                                    p_cur_history_epoch);

            std::advance(it_cur, 1); //advance to next element

            if (it_cur != p_history_line.end()) {
                if (epoch_to_delete)
                  for (auto it_i = it_cur; it_i != std::end(p_history_line); it_i++)
                    (*epoch_to_delete).get().push_back(*it_i);
                p_history_line.erase(it_cur, std::end(p_history_line));
              }

          }

        EPOCH_TYPE new_epoch_ex =
            (new_epoch && *new_epoch > p_max_history_epoch ? *new_epoch : p_max_history_epoch + 1);

        p_history_line.push_back(new_epoch_ex);
        p_max_history_epoch = new_epoch_ex;
        p_cur_history_epoch = new_epoch_ex;

        return std::optional<EPOCH_TYPE>(new_epoch_ex);

      }

    };

    /**********************************************************************************************
     *
     * history_stream_t<STORED_TYPE>
     *
     *********************************************************************************************/
    template<typename STORED_TYPE, typename EPOCH_TYPE = size_t>
    class history_stream_t : public history_stream_base_t<EPOCH_TYPE> {

    public:

      using history_line_t = typename history_stream_base_t<EPOCH_TYPE>::history_line_t;
      using ref_history_line_t = typename history_stream_base_t<EPOCH_TYPE>::ref_history_line_t;

    private:

      friend history_stream_base_t<EPOCH_TYPE>;
      std::map<EPOCH_TYPE, STORED_TYPE> p_value_stream{{0, STORED_TYPE()}};
      STORED_TYPE p_cur_value;

    public:

      void push_epoch_with_data(STORED_TYPE &&new_value,
                                std::optional<size_t> new_epoch = std::nullopt) {

        history_line_t hline;
        auto new_epoch_opt =
            history_stream_base_t<EPOCH_TYPE>::push_epoch(new_epoch,
                                                          std::optional<ref_history_line_t>(hline));

        if (new_epoch_opt)
          p_value_stream.insert({*new_epoch_opt, std::forward<STORED_TYPE>(new_value)});

        for (auto hl_elem : hline)
          p_value_stream.erase(hl_elem);

      }

      std::optional<STORED_TYPE> get_epoch_value(EPOCH_TYPE epoch_id) {
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

        for (size_t i = 0; i < history_stream_base_t<EPOCH_TYPE>::get_hist_size(); i++)
          std::cout << "Epoch id = "
                    << history_stream_base_t<EPOCH_TYPE>::get_hist_at(i)
                    << ", value = "
                    << p_value_stream[history_stream_base_t<EPOCH_TYPE>::get_hist_at(i)]
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
