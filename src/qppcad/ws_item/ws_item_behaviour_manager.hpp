#ifndef QPP_CAD_WS_ITEM_BHV_MGR
#define QPP_CAD_WS_ITEM_BHV_MGR

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/ws_item_extended_editor.hpp>

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

    /**
     * @brief The ws_item_io_file_format_group_t class
     */
    class ws_item_io_file_format_group_t {

      public:

        std::string m_full_name;
        std::string m_short_name;
        std::set<size_t> m_ffs_lookup;

    };

    /**
     * @brief The ws_item_io_file_format_t class
     */
    class ws_item_io_file_format_t {

      public:

        std::string m_full_name;
        std::string m_shortname;
        std::vector<std::string> m_finger_prints;
        std::optional<size_t> m_group_hash;

    };

    /**
     * @brief The ws_item_tool_t class
     */
    class ws_item_tool_t {

    public:

         /**
         * @brief exec
         * @param item
         */
      virtual void exec(ws_item_t *item) = 0;

    };

    /**
     * @brief The ws_item_tool_group_t class
     */
    class ws_item_tool_group_t {

      public:

        std::string m_full_name;
    };

    /**
     * @brief The ws_item_tool_info_t class
     */
    class ws_item_tool_info_t {

      public:

        std::string m_full_name;
        size_t m_group_hash;
        size_t m_accepted_type;
        bool m_item_required{true};
        std::function<std::shared_ptr<ws_item_tool_t>() > m_fabric;

    };

    /**
     * @brief The ws_item_io_behaviour_t class
     */
    class ws_item_io_behaviour_t {

      public:

        size_t m_accepted_type;
        size_t m_accepted_file_format;
        bool m_menu_occupier{true};
        bool m_can_be_imported_as_new_ws{true};
        bool m_can_be_imported_to_ws{true};
        bool m_support_multi_files{false};

        virtual bool can_save() = 0;
        virtual bool can_load() = 0;

        virtual bool check_before_save(ws_item_t *item, std::string &message) {
          return true;
        }

        virtual void load_from_stream(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                      ws_item_t *_item,
                                      workspace_t *ws) = 0;

        virtual void save_to_stream(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                                    ws_item_t *_item) = 0;

        bool is_type_accepted(size_t _type);

    };

    template <typename T>
    class ws_item_io_inherited_bhv_t : public ws_item_io_behaviour_t {

      public:

        bool check_before_save(ws_item_t *_item, std::string &message) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              return check_before_save_ex(casted_item, message);
            }
          return true;
        }

        virtual bool check_before_save_ex(T *_item, std::string &message) {
          return true;
        }

        void load_from_stream(std::basic_istream<CHAR_EX,TRAITS> &stream,
                              ws_item_t *_item,
                              workspace_t *ws) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) load_from_stream_ex(stream, casted_item, ws);
            }
        }

        virtual void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                         T *_item,
                                         workspace_t *ws) = 0;

        void save_to_stream(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                            ws_item_t *_item) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) save_to_stream_ex(stream, casted_item);
            }
        }

        virtual void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                                       T *_item) = 0;

    };

    template <typename T>
    class ws_item_io_inherited_bhv_hooked_t : public ws_item_io_behaviour_t {

      public:

        bool check_before_save(ws_item_t *_item, std::string &message) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              return check_before_save_ex(casted_item, message);
            }
          return true;
        }

        virtual bool check_before_save_ex(T *_item, std::string &message) {
          return true;
        }

        virtual void pre_load_hook(T *_item, workspace_t *ws)  = 0;
        virtual void post_load_hook(T *_item, workspace_t *ws)  = 0;
        virtual void pre_save_hook(T *_item)  = 0;
        virtual void post_save_hook(T *_item)  = 0;

        void load_from_stream(std::basic_istream<CHAR_EX,TRAITS> &stream,
                              ws_item_t *_item,
                              workspace_t *ws) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) {
                  pre_load_hook(casted_item, ws);
                  load_from_stream_ex(stream, casted_item, ws);
                  post_load_hook(casted_item, ws);
                }
            }
        }

        virtual void load_from_stream_ex(std::basic_istream<CHAR_EX,TRAITS> &stream,
                                         T *_item,
                                         workspace_t *ws) = 0;

        void save_to_stream(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                            ws_item_t *_item) override {
          if (_item && _item->get_type() == T::get_type_static()) {
              T* casted_item = _item->cast_as<T>();
              if (casted_item) {
                  pre_save_hook(casted_item);
                  save_to_stream_ex(stream, casted_item);
                  post_save_hook(casted_item);
                }
            }
        }

        virtual void save_to_stream_ex(std::basic_ostream<CHAR_EX,TRAITS> &stream,
                                       T *_item) = 0;

    };

    /**
     * @brief The ws_item_behaviour_manager_t class
     */
    class ws_item_behaviour_manager_t {

      public:

        /** @brief ws_item_behaviour_manager_t */
        ws_item_behaviour_manager_t();

        /** @brief m_file_formats */
        std::map<size_t, ws_item_io_file_format_t> m_file_formats;

        /** @brief m_file_format_groups */
        std::map<size_t, ws_item_io_file_format_group_t> m_file_format_groups;

        /** @brief m_fabric_ws_item */
        std::map<size_t, std::function<std::shared_ptr<ws_item_t>() > > m_fabric_ws_item;

        /** @brief m_obj_insp_widgets */
        std::map<size_t, std::shared_ptr<ws_item_obj_insp_widget_t> > m_obj_insp_widgets;

        /** @brief m_obj_insp_fabric */
        std::map<size_t,
        std::function<std::shared_ptr<ws_item_obj_insp_widget_t>() > > m_obj_insp_fabric;

        /** @brief m_ext_editors */
        std::map<size_t, std::shared_ptr<ws_item_extended_editor_t> > m_ext_editors;

        /** @brief m_ext_editors_fabric */
        std::map<size_t,
        std::function<std::shared_ptr<ws_item_extended_editor_t>() > > m_ext_editors_fabric;

        /** @brief m_ws_item_io */
        std::vector<std::shared_ptr<ws_item_io_behaviour_t> > m_ws_item_io;

        /** @brief m_tools_groups */
        std::map<size_t, ws_item_tool_group_t> m_tools_groups;

        /** @brief m_tools_info */
        std::map<size_t, ws_item_tool_info_t> m_tools_info;

        /**
         * @brief load_ws_itm_from_file
         * @param file_name
         * @param io_bhv_idx
         * @param ws
         * @return
         */
        std::shared_ptr<ws_item_t> load_ws_itm_from_file(const std::string &file_name,
                                                          size_t io_bhv_idx,
                                                          workspace_t *ws);

        /**
         * @brief load_ws_itm_from_file
         * @param file_name
         * @param ws
         * @return
         */
        std::shared_ptr<ws_item_t> load_ws_itm_from_file(const std::string &file_name,
                                                          workspace_t *ws);

        /**
         * @brief save_ws_itm_to_file
         * @param file_name
         * @param ws_item
         * @param bhv_id
         * @param message
         * @return
         */
        bool save_ws_itm_to_file(std::string &file_name,
                                 std::shared_ptr<ws_item_t> ws_item,
                                 size_t bhv_id,
                                 std::string &message);

        /**
         * @brief get_ff_full_name
         * @param _file_format_hash
         * @return
         */
        std::string get_ff_full_name(size_t _file_format_hash);

        /**
         * @brief get_ff_short_name
         * @param _file_format_hash
         * @return
         */
        std::string get_ff_short_name(size_t _file_format_hash);

        /**
         * @brief reg_ff
         * @param _full_name
         * @param _short_name
         * @param _file_format_group_hash
         * @param _finger_prints
         * @return
         */
        size_t reg_ff(std::string _full_name,
                      std::string _short_name,
                      size_t _file_format_group_hash,
                      std::vector<std::string> _finger_prints);

        /**
         * @brief reg_ffg
         * @param _full_name
         * @param _short_name
         * @return
         */
        size_t reg_ffg(std::string _full_name,
                       std::string _short_name);

        /**
         * @brief get_ff_by_finger_print
         * @param file_name
         * @return
         */
        std::optional<size_t> get_ff_by_finger_print(const std::string &file_name);

        /**
         * @brief get_ff_by_short_name
         * @param ffmt_short_name
         * @return
         */
        std::optional<size_t> get_ff_by_short_name(const std::string &ffmt_short_name);

        /**
         * @brief get_io_bhv_by_file_format
         * @param file_format
         * @return
         */
        std::optional<size_t> get_io_bhv_by_file_format(size_t file_format);

        /**
         * @brief get_io_bhv_by_file_format_ex
         * @param file_format
         * @param type_hash
         * @return
         */
        std::optional<size_t> get_io_bhv_by_file_format_ex(size_t file_format,
                                                           size_t type_hash);

        /**
         * @brief reg_io_bhv
         * @param io_bhv_inst
         * @param accepted_file_format
         * @param accepted_type
         */
        void reg_io_bhv(std::shared_ptr<ws_item_io_behaviour_t> io_bhv_inst,
                        size_t accepted_file_format,
                        size_t accepted_type);

        /**
         * @brief unreg_ff
         * @param _file_format_hash
         */
        void unreg_ff(size_t _file_format_hash);

        /**
         * @brief reg_item_fbr
         * @param hash
         * @param func
         */
        void reg_item_fbr(size_t hash, std::function<std::shared_ptr<ws_item_t>()> func);

        /**
         * @brief reg_obj_insp_fbr
         * @param hash
         * @param func
         */
        void reg_obj_insp_fbr(size_t hash,
                              std::function<std::shared_ptr<ws_item_obj_insp_widget_t>()> func);

        /**
         * @brief reg_ext_editor_fbr
         * @param hash
         * @param func
         */
        void reg_ext_editor_fbr(
            size_t hash, std::function<std::shared_ptr<ws_item_extended_editor_t>()> func);

        /**
         * @brief is_obj_insp_fbr_exists
         * @param hash
         * @return
         */
        bool is_obj_insp_fbr_exists(size_t hash);

        /**
         * @brief is_obj_insp_exists
         * @param hash
         * @return
         */
        bool is_obj_insp_exists(size_t hash);

        /**
         * @brief get_obj_insp_widget_sp
         * @param hash
         * @return
         */
        std::shared_ptr<ws_item_obj_insp_widget_t> get_obj_insp_widget_sp(size_t hash);

        /**
         * @brief get_ext_editor_widget_sp
         * @param hash
         * @param ed_order
         * @return
         */
        std::shared_ptr<ws_item_extended_editor_t> get_ext_editor_widget_sp(size_t hash,
                                                                            size_t ed_order = 0);

        /**
         * @brief fabric_by_type
         * @param type_id
         * @return
         */
        std::shared_ptr<ws_item_t> fabric_by_type(size_t type_id);

        /**
         * @brief ws_item_fbr_by_type_p
         * @param type_id
         * @return
         */
        ws_item_t* ws_item_fbr_by_type_p(size_t type_id);

        /**
         * @brief reg_tool_grp
         * @param _full_name
         * @return
         */
        size_t reg_tool_grp(std::string _full_name);

        /**
         * @brief reg_tool
         * @param _full_name
         * @param _g_hash
         * @param _t_hash
         * @param _itm_req
         * @param _fabric
         * @return
         */
        size_t reg_tool(std::string _full_name,
                        size_t _g_hash,
                        size_t _t_hash,
                        bool _itm_req,
                        std::function<std::shared_ptr<ws_item_tool_t>() > _fabric);

        /**
         * @brief exec_tool
         * @param item
         * @param tool_hash
         */
        void exec_tool(ws_item_t* item, size_t tool_hash);

    };

  }

}


#endif
