#ifndef QPP_CAD_WS_ITEM_BHV_MGR
#define QPP_CAD_WS_ITEM_BHV_MGR

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item.hpp>

namespace qpp {

  namespace cad {

    enum ws_item_io_status {
      io_status_succes,
      io_status_file_doesnt_exists,
      io_status_invalid_file_format,
      io_status_file_format_not_supported,
      io_status_file_format_not_supports_saving,
      io_status_file_format_not_supports_loading,
      io_status_invalid_type,
      io_status_parsing_error
    };


    class workspace_t; //fwd decl

    class ws_item_io_file_format_group_t {
      public:
        std::string m_full_name;
        std::string m_short_name;
        std::set<size_t> m_ffs_lookup;
    };

    class ws_item_io_file_format_t {
      public:
        std::string m_full_name;
        std::string m_shortname;
        std::vector<std::string> m_finger_prints;
        std::optional<size_t> m_group_hash;
    };

    class ws_item_io_behaviour_t {
      public:
        size_t m_accepted_type;
        size_t m_accepted_file_format;
        bool m_menu_occupier{true};
        bool m_can_be_imported_as_new_ws{true};
        bool m_can_be_imported_to_ws{true};

        virtual bool can_save() = 0;
        virtual bool can_load() = 0;

        virtual void load_from_stream(std::basic_istream<CHAR,TRAITS> &stream,
                                      ws_item_t *_item,
                                      workspace_t *ws) = 0;

        virtual void save_to_stream(std::basic_ostream<CHAR,TRAITS> &stream,
                                    ws_item_t *_item) = 0;

        bool is_type_accepted(size_t _type);
    };

    template <typename T>
    class ws_item_io_inherited_bhv_t : public ws_item_io_behaviour_t {

      public:

        void load_from_stream(std::basic_istream<CHAR,TRAITS> &stream,
                              ws_item_t *_item,
                              workspace_t *ws) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) load_from_stream_ex(stream, casted_item, ws);
            }
        }

        virtual void load_from_stream_ex(std::basic_istream<CHAR,TRAITS> &stream,
                                         T *_item,
                                         workspace_t *ws) = 0;

        void save_to_stream(std::basic_ostream<CHAR,TRAITS> &stream,
                            ws_item_t *_item) override {
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
        std::map<size_t, ws_item_io_file_format_group_t> m_file_format_groups;
        std::vector<std::shared_ptr<ws_item_io_behaviour_t> > m_ws_item_io;

        std::shared_ptr<ws_item_t> load_ws_item_from_file(std::string &file_name,
                                                          size_t io_bhv_idx,
                                                          workspace_t *ws);

        std::shared_ptr<ws_item_t> load_ws_item_from_file(std::string &file_name,
                                                          workspace_t *ws);

        void save_ws_item_to_file(std::string &file_name,
                                  ws_item_t *ws_item,
                                  size_t file_format,
                                  bool trust = false);

        void save_ws_item_to_file(std::string &file_name,
                                  ws_item_t *ws_item);

        std::string get_file_format_full_name(size_t _file_format_hash);

        size_t reg_ff(std::string _full_name,
                      std::string _short_name,
                      size_t _file_format_group_hash,
                      std::vector<std::string> _finger_prints);

        size_t reg_ffg(std::string _full_name,
                       std::string _short_name);

        std::optional<size_t> get_file_format(std::string &file_name);

        std::optional<size_t> get_io_bhv_by_file_format(size_t file_format);
        std::optional<size_t> get_io_bhv_by_file_format_ex(size_t file_format,
                                                           size_t type_hash);

        void reg_io_bhv(std::shared_ptr<ws_item_io_behaviour_t> io_bhv_inst,
                        size_t accepted_file_format,
                        size_t accepted_type);

        void unregister_file_format(size_t _file_format_hash);

        std::shared_ptr<ws_item_t> fabric_by_type(size_t type_id);
        ws_item_t* fabric_by_type_p(size_t type_id);
    };

  }

}


#endif
