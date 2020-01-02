#ifndef QPPCAD_WS_ITEM_BHV_MGR
#define QPPCAD_WS_ITEM_BHV_MGR

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ws_item/ws_item_obj_insp_widget.hpp>
#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/sflow_behaviour_manager.hpp>
#include <qppcad/core/fixture.hpp>
#include <qppcad/ui/toolbar_element.hpp>
#include <qppcad/ui/ws_item_inline_tool_widget.hpp>

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

    enum ws_item_tool_type_e {

      ws_item_tool_invalid,
      ws_item_tool_modal,
      ws_item_tool_inline_horizontal,
      ws_item_tool_inline_vertical,
      ws_item_tool_popup

    };


    class workspace_t; //fwd decl

    /**
     * @brief The ws_item_io_file_format_group_t class
     */
    struct ws_item_io_file_format_group_t {

      std::string m_full_name;
      std::string m_short_name;
      std::set<size_t> m_ffs_lookup;

    };

    /**
     * @brief The ws_item_io_file_format_t class
     */
    struct ws_item_io_file_format_t {

      std::string m_full_name;
      std::string m_shortname;
      std::vector<std::string> m_finger_prints;
      std::optional<size_t> m_group_hash;

    };

    /**
     * @brief The ws_item_tool_t class
     */
    struct ws_item_tool_t {

      virtual void exec(ws_item_t *item, uint32_t _error_ctx) = 0;
      virtual ws_item_inline_tool_widget_t* construct_inline_tool() {return nullptr;}

    };

    /**
     * @brief The ws_item_tool_group_t class
     */
    struct ws_item_tool_group_t {
      std::string m_full_name;
    };

    /**
     * @brief The ws_item_tool_info_t class
     */
    struct ws_item_tool_info_t {

      std::string m_full_name;
      size_t m_group_hash;
      size_t m_accepted_type;
      ws_item_tool_type_e m_tool_type{ws_item_tool_type_e::ws_item_tool_modal};
      bool m_show_in_menu{true};
      bool m_item_required{true};
      std::function<std::shared_ptr<ws_item_tool_t>() > m_fabric;

    };

    /**
     * @brief The ws_item_extended_editor_info_t class
     */
    struct ws_item_extended_editor_info_t {

      std::string m_full_name;
      size_t m_type;
      size_t m_order;
      std::function<std::shared_ptr<ws_item_extended_editor_t>() > m_fabric;
      std::shared_ptr<ws_item_extended_editor_t> m_inst{nullptr};

    };

    /**
     * @brief The ws_item_io_behaviour_t class
     */
    struct ws_item_io_behaviour_t {

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

      /* ws_item_behaviour_manager_t */
      ws_item_behaviour_manager_t();

      /* m_file_formats */
      std::map<size_t, ws_item_io_file_format_t> m_file_formats;

      /* m_file_format_groups */
      std::map<size_t, ws_item_io_file_format_group_t> m_file_format_groups;

      /* m_fabric_ws_item */
      std::map<size_t, std::function<std::shared_ptr<ws_item_t>() > > m_fabric_ws_item;

      /* m_obj_insp_widgets */
      std::map<size_t, std::shared_ptr<ws_item_obj_insp_widget_t> > m_obj_insp_widgets;

      /* m_obj_insp_fabric */
      std::map<size_t,
      std::function<std::shared_ptr<ws_item_obj_insp_widget_t>() > > m_obj_insp_fabric;

      /* m_ext_editors */
      std::vector<std::shared_ptr<ws_item_extended_editor_t> > m_ext_editors;
      std::map<std::tuple<size_t, size_t>, ws_item_extended_editor_info_t> m_ext_editors_info;

      /* m_ws_item_io */
      std::vector<std::shared_ptr<ws_item_io_behaviour_t> > m_ws_item_io;

      /* m_tools_groups */
      std::map<size_t, ws_item_tool_group_t> m_tools_groups;

      /* m_tools_info */
      std::map<size_t, ws_item_tool_info_t> m_tools_info;

      /* sflow framework info */
      std::map<size_t, sflow_node_group_info_t> m_sflow_node_group_info;
      std::map<size_t, sflow_node_info_t> m_sflow_node_info;

      /* fixtures framework info */
      std::map<size_t, fixture_info_t> m_fixtures_info;

      /* toolbar_element framework info*/
      std::map<size_t, toolbar_element_info_t> m_toolbar_elements_info;

      void load_fixtures_from_path(const std::vector<std::string> &file_paths);

      std::shared_ptr<ws_item_t> load_ws_itm_from_file(const std::string &file_name,
                                                       size_t io_bhv_idx,
                                                       workspace_t *ws);

      std::shared_ptr<ws_item_t> load_ws_itm_from_file(const std::string &file_name,
                                                       workspace_t *ws);

      bool save_ws_itm_to_file(std::string &file_name,
                               std::shared_ptr<ws_item_t> ws_item,
                               size_t bhv_id,
                               std::string &message);

      std::string get_ff_full_name(size_t _file_format_hash);
      std::string get_ff_short_name(size_t _file_format_hash);


      size_t reg_ff(std::string _full_name, std::string _short_name,
                    size_t _file_format_group_hash, std::vector<std::string> _finger_prints);
      void unreg_ff(size_t _file_format_hash);

      size_t reg_ffg(std::string _full_name, std::string _short_name);

      std::optional<size_t> get_ff_by_finger_print(const std::string &file_name);
      std::optional<size_t> get_ff_by_short_name(const std::string &ffmt_short_name);

      std::optional<size_t> get_io_bhv_by_file_format(size_t file_format);
      std::optional<size_t> get_io_bhv_by_file_format_ex(size_t file_format, size_t type_hash);

      void reg_io_bhv(std::shared_ptr<ws_item_io_behaviour_t> io_bhv_inst,
                      size_t accepted_file_format,
                      size_t accepted_type);

      void reg_item_fbr(size_t hash, std::function<std::shared_ptr<ws_item_t>()> func);

      void reg_obj_insp_fbr(size_t hash,
                            std::function<std::shared_ptr<ws_item_obj_insp_widget_t>()> func);

      void reg_ext_editor_fbr(size_t hash,
                              size_t editor_order,
                              std::string editor_name,
                              std::function<std::shared_ptr<ws_item_extended_editor_t>()> func);

      void reg_toolbar_elem_fbr(size_t hash,
                                std::string editor_name,
                                std::function<std::shared_ptr<toolbar_element_t>()> func);

      bool is_obj_insp_fbr_exists(size_t hash);

      bool is_obj_insp_exists(size_t hash);

      std::shared_ptr<ws_item_obj_insp_widget_t> get_obj_insp_widget_sp(size_t hash);

      void cache_obj_insp_widgets();

      std::shared_ptr<ws_item_extended_editor_t> get_ext_editor_widget_sp(size_t hash,
                                                                          size_t ed_order = 0);

      size_t get_ext_editor_info(size_t hash);

      std::shared_ptr<ws_item_t> fbr_ws_item_by_type(size_t type_id);
      std::shared_ptr<ws_item_t> fbr_ws_item_by_name(const std::string &type_name);

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
                      size_t grgp_hash,
                      size_t type_hash,
                      bool _itm_req,
                      ws_item_tool_type_e tool_type,
                      std::function<std::shared_ptr<ws_item_tool_t>() > _fabric);

      size_t reg_sflow_grp(std::string group_name);
      size_t reg_reg_sf_fbr(std::string _full_name,
                            size_t _g_hash,
                            std::function<std::shared_ptr<sflow_node_t>() > _fabric);
      /**
       * @brief exec_tool
       * @param item
       * @param tool_hash
       * @param _error_ctx
       */
      void exec_tool(ws_item_t* item, size_t tool_hash, uint32_t _error_ctx = error_ctx_mbox);
      ws_item_tool_type_e get_tool_type(size_t tool_hash);

      /**
       * @brief exec_tool_by_name
       * @param tool_name
       * @param item
       * @param _error_ctx
       */
      void exec_tool_by_name(std::string tool_name,
                             ws_item_t *item = nullptr,
                             uint32_t _error_ctx = error_ctx_mbox);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
