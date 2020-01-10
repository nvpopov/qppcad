#ifndef QPPCAD_WORKSPACE_H
#define QPPCAD_WORKSPACE_H

#include <qppcad/core/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/render/camera.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/core/gizmo.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class workspace_t;
    class workspace_manager_t;
    class ws_item_t;

    class app_state_t;

    /**
     * @brief The workspace_t class
     */
    class workspace_t :
        public qpp_object_t,
        public std::enable_shared_from_this<workspace_t> {

        QPP_OBJECT(workspace_t, qpp_object_t)

      public:

        workspace_manager_t                      *m_owner{nullptr};
        std::string                              m_fs_path{""};
        ws_edit_e                                m_edit_type{ws_edit_e::edit_item};
        std::vector<std::shared_ptr<ws_item_t> > m_ws_items;
        std::string                              m_ws_name;
        std::unique_ptr<camera_t>                m_camera;
        ray_t<float>                             m_ray;
        std::unique_ptr<gizmo_t>                 m_gizmo;
        vector3<float>                           m_bg_color{1, 1, 1};
        bool m_need_to_update_overview{false};
        bool m_show_obj_insp{true};
        bool m_is_ws_imported{false};
        bool m_marked_for_deletion{false};
        bool m_first_render{true};
        bool m_scenic_rotation{false};
        vector3<float> m_scenic_rotation_speed{0.0f, 0.0f, 0.1f};

        workspace_t(std::string _ws_name = "default") {
          m_ws_name = _ws_name;
          m_camera = std::make_unique<camera_t>();
          m_camera->reset_camera();
          m_gizmo = std::make_unique<gizmo_t>();
        }

        opt<size_t>  get_sel_idx();
        ws_item_t *get_sel();
        std::shared_ptr<ws_item_t> get_sel_sp();
        std::shared_ptr<ws_item_t> get_by_name(std::string _name);

        template<typename T>
        T* get_sel_as() {
          auto cur_it = get_sel();
          return  cur_it->cast_as<T>();
        }

        bool set_sel_item(const size_t sel_idx, bool emit_signal = true);
        void next_item();
        void prev_item();
        void unsel_all(bool emit_signal = true);
        void toggle_edit_mode();
        void ws_changed();
        void reset_camera();
        void set_best_view();
        void render();
        void render_overlay(QPainter &painter);
        void mouse_click(const float mouse_x, const float mouse_y);
        void mouse_double_click(const float mouse_x, const float mouse_y);
        void add_item_to_ws(const std::shared_ptr<ws_item_t> item_to_add);
        void update_overview(const std::string &overview_text);

        void clear_connected_items(std::shared_ptr<ws_item_t> item_to_delete);
        void save_ws_to_json(const std::string filename);
        void load_ws_from_json(const std::string filename);
        void update(float delta_time);
        void set_edit_type(const ws_edit_e new_edit_type);
        void copy_camera(std::shared_ptr<workspace_t> source);
        void push_cam_state();
        void pop_cam_state();
        void del_item_by_index(size_t idx);
        void make_overview_dirty();
        std::string py_get_repr();
        std::shared_ptr<ws_item_t> py_construct_item(std::string class_name,
                                                     std::string item_name);

    };

    /// \brief The workspace_manager_t class
    class workspace_manager_t : public std::enable_shared_from_this<workspace_manager_t> {

      private:

        app_state_t *cached_astate;
        opt<size_t> m_cur_ws_id{};

      public:

        std::vector<std::shared_ptr<workspace_t> > m_ws;
        std::shared_ptr<ws_item_behaviour_manager_t> m_bhv_mgr;
        workspace_manager_t(app_state_t *_astate);

        std::shared_ptr<workspace_t> get_cur_ws();
        std::shared_ptr<workspace_t> get_by_name(std::string target_name);
        opt<size_t> get_cur_id();

        bool set_cur_id(const opt<size_t> ws_index);

        template<int I>
        void force_set_cur_ws(){set_cur_id(opt<size_t>(I));}

        std::shared_ptr<workspace_t> get_ws(int id);
        void next_ws();
        void prev_ws();
        void cur_ws_next_item();
        void cur_ws_prev_item();
        bool has_wss(){return m_ws.size()>0;}
        void init_default();
        void render_cur_ws();
        void render_cur_ws_overlay(QPainter &painter);
        void mouse_click();
        void mouse_double_click();
        void ws_mgr_changed();
        void add_ws(const std::shared_ptr<workspace_t> &ws_to_add);
        void init_ws_item_bhv_mgr();
        void move_ws(size_t from, size_t to);
        void load_from_file_autodeduce(const std::string &file_name,
                                       const std::string &file_format = "",
                                       bool create_new_ws = true);

        void load_from_file(const std::string &fname, bool override = true);

        void import_from_file(const std::string &fname,
                              size_t bhv_id,
                              bool need_to_create_new_ws = true);

        void save_ws_item_to_file(std::string &file_name,
                                  std::shared_ptr<ws_item_t> ws_item,
                                  size_t bhv_id); //dispatch to bhv_mgr

        void create_new_ws(bool switch_to_new_workspace = true);

        std::shared_ptr<ws_item_t> get_sel_itm_sp();

        void utility_event_loop();

        /**
         * @brief get_sel_tuple
         * @param _error_context
         * @return
         */
        template<typename T>
        std::tuple<std::shared_ptr<workspace_t>, std::shared_ptr<ws_item_t>, T*>
        get_sel_tpl_itm(uint32_t _error_context = error_ctx_def) {

          auto cur_ws = get_cur_ws();
          if (!cur_ws) {
              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("Invalid workspace!");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("Invalid workspace!"));
              return {nullptr, nullptr, nullptr};
            }

          auto cur_it = cur_ws->get_sel_sp();
          if (!cur_it) {
              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("No item selected in the workspace");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("No item selected in the workspace"));
              return {cur_ws, nullptr, nullptr};
            }

          auto casted_it = cur_it->cast_as<T>();
          if (!casted_it) {
              if (_error_context & error_ctx_throw) {
                  throw std::invalid_argument(
                        fmt::format("Cannot cast types: from {} to {}",
                                    cur_it->get_type_name(), T::get_type_name_static())
                        );
                }
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("Cannot cast types: from %1 to %2")
                                     .arg(QString::fromStdString(cur_it->get_type_name()))
                                     .arg(QString::fromStdString(T::get_type_name_static()))
                                     );
              return  {cur_ws, cur_it, nullptr};
            }

          return {cur_ws, cur_it, casted_it};

        }

        /**
         * @brief get_sel_tpl_itmc
         * @param _error_context
         * @return
         */
        template<typename T>
        std::tuple<std::shared_ptr<workspace_t>, std::shared_ptr<ws_item_t>, T*, bool>
        get_sel_tpl_itmc(uint32_t _error_context = error_ctx_def) {

          auto cur_ws = get_cur_ws();

          if (!cur_ws) {

              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("Invalid workspace!");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("Invalid workspace!"));
              return {nullptr, nullptr, nullptr, false};

            }

          auto cur_it = cur_ws->get_sel_sp();

          if (!cur_it) {

              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("No item selected in the workspace");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("No item selected in the workspace"));
              return {cur_ws, nullptr, nullptr, false};

            }

          auto casted_it = cur_it->cast_as<T>();

          if (!casted_it) {

              if (_error_context & error_ctx_throw) {
                  throw std::invalid_argument(
                        fmt::format("Cannot cast types: from {} to {}",
                                    cur_it->get_type_name(), T::get_type_name_static())
                        );
                }

              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("Cannot cast types: from %1 to %2")
                                     .arg(QString::fromStdString(cur_it->get_type_name()))
                                     .arg(QString::fromStdString(T::get_type_name_static()))
                                     );

              return  {cur_ws, cur_it, nullptr, false};

            }

          return {cur_ws, cur_it, casted_it, true};

        }

        /**
         * @brief get selection tuple without casting
         * @param _error_context
         * @return
         */
        std::tuple<std::shared_ptr<workspace_t>, std::shared_ptr<ws_item_t>, bool >
        get_sel_tpl_itm_nc(uint32_t _error_context = error_ctx_def) {

          auto cur_ws = get_cur_ws();

          if (!cur_ws) {

              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("Invalid workspace!");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("Invalid workspace!"));
              return {nullptr, nullptr, false};

            }

          auto cur_it = cur_ws->get_sel_sp();

          if (!cur_it) {

              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("No item selected in the workspace");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("No item selected in the workspace"));
              return {cur_ws, nullptr, false};

            }

          return {cur_ws, cur_it, true};

        }

        /**
         * @brief get_context
         * @param _error_context
         * @return
         */
        std::tuple<bool, std::shared_ptr<workspace_t> >
        get_sel_tuple_ws(uint32_t _error_context = error_ctx_def) {

          if (!has_wss()) {

              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("There are`t workspaces in the field");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("There are`t workspaces in the field"));
              return {false, nullptr};

            }

          auto cur_ws = get_cur_ws();

          if (!cur_ws) {

              if (_error_context & error_ctx_throw)
                throw std::invalid_argument("Invalid workspace!");
              if (_error_context & error_ctx_mbox)
                QMessageBox::warning(nullptr,
                                     QObject::tr("Error"),
                                     QObject::tr("Invalid workspace!"));
              return {true, nullptr};

            }

          return {true, cur_ws};

        }

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
