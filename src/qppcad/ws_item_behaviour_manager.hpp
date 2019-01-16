#ifndef QPP_CAD_WS_ITEM_BHV_MGR
#define QPP_CAD_WS_ITEM_BHV_MGR

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>

namespace qpp {

  namespace cad {

    class ws_item_io_file_format_t {
      public:
        std::string m_full_name;
        std::string m_shortname;
    };

    class ws_item_io_behaviour_t {
      public:
        size_t m_accepted_type;
        size_t m_accepted_file_format;
        virtual bool can_save() = 0;
        virtual bool can_load() = 0;
        virtual void load_from_stream(std::basic_istream<CHAR,TRAITS> &stream,
                                      ws_item_t *_item) = 0;
        virtual void save_to_stream(std::basic_ostream<CHAR,TRAITS> &stream,
                                    ws_item_t *_item) = 0;
        virtual bool deduce_from_file_name(std::string &file_name);
        bool is_type_accepted(size_t _type);
    };

    template <typename T>
    class ws_item_io_inherited_bhv_t : public ws_item_io_behaviour_t {

      public:

        void load_from_stream(std::basic_istream<CHAR,TRAITS> &stream, ws_item_t *_item) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) load_from_stream_ex(stream, casted_item);
            }
        }
        virtual void load_from_stream_ex(std::basic_istream<CHAR,TRAITS> &stream,
                                         T *_item) = 0;

        void save_to_stream(std::basic_ostream<CHAR,TRAITS> &stream, ws_item_t *_item) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) save_to_stream_ex(stream, casted_item);
            }
        }
        virtual void save_to_stream_ex(std::basic_ostream<CHAR,TRAITS> &stream,
                                         T *_item) = 0;

    };

    class ws_item_behaviour_manager_t {
      private:

      public:
        ws_item_behaviour_manager_t();
        std::map<size_t, ws_item_io_file_format_t> m_file_formats;
        std::vector<std::shared_ptr<ws_item_io_behaviour_t> > m_ws_item_io;


        void load_ws_item_from_file(std::string &file_name, ws_item_t *ws_item,
                                    size_t file_format, bool trust = false);

        //auto deduce
        void load_ws_item_from_file(std::string &file_name, ws_item_t *ws_item);

        void save_ws_item_to_file(std::string &file_name, ws_item_t *ws_item,
                                  size_t file_format, bool trust = false);

        //auto deduce
        void save_ws_item_to_file(std::string &file_name, ws_item_t *ws_item);
        std::string get_file_format_full_name(size_t _file_format_hash);

        size_t register_file_format(std::string _full_name,
                                    std::string _short_name);

        void register_io_behaviour(std::shared_ptr<ws_item_io_behaviour_t> io_bhv_inst,
                                   size_t accepted_file_format,
                                   size_t accepted_type);

        void unregister_file_format(size_t _file_format_hash);

        std::shared_ptr<ws_item_t> fabric_by_type(size_t type_id);
    };

  }

}


#endif
