#ifndef QPP_WORKSPACE_H
#define QPP_WORKSPACE_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/gizmo.hpp>

namespace qpp {

  namespace cad {

    class workspace_t;
    class ws_item_t;

    class app_state_t;

    /// \brief The workspace_manager_t class

    class workspace_manager_t : public std::enable_shared_from_this<workspace_manager_t> {
      private:
        app_state_t *cached_astate;
        size_t m_cur_ws_id;

      public:
        std::vector<std::shared_ptr<workspace_t> > m_ws;

        workspace_manager_t(app_state_t *_astate);

        std::shared_ptr<workspace_t> get_cur_ws();
        std::optional<size_t> get_cur_id();

        bool set_cur_id(const size_t ws_index);

        template<int I>
        void force_set_cur_ws(){set_cur_id(I);}

        std::shared_ptr<workspace_t> get_ws(int id);
        bool has_wss(){return m_ws.size()>0;}
        void init_default();
        void render_cur_ws();
        void render_cur_ws_overlay(QPainter &painter);
        void mouse_click();
        void ws_mgr_changed();
        void add_ws(const std::shared_ptr<workspace_t> &ws_to_add);

        void load_from_file_autodeduce(const std::string file_name,
                                       const std::string file_format = "");

        void load_from_file(const std::string &fname,
                            qc_file_fmt file_format,
                            bool override = true);


        void create_new_ws(bool switch_to_new_workspace = true);

    };

    ///
    /// \brief The workspace_t class
    ///
    class workspace_t : public std::enable_shared_from_this<workspace_t> {
      public:
        workspace_manager_t                      *m_owner{nullptr};
        std::string                              m_fs_path{""};
        ws_edit_t                             m_edit_type{ws_edit_t::edit_item};
        std::vector<std::shared_ptr<ws_item_t> > m_ws_items;
        std::string                              m_ws_name;
        std::unique_ptr<camera_t>                m_camera;
        ray_t<float>                             m_ray;
        bool                                     m_first_render{true};
        std::unique_ptr<gizmo_t>                 m_gizmo;
        vector3<float>                           m_background_color{1, 1, 1};
        bool m_is_ws_imported{false};

        workspace_t(std::string _ws_name = "default") {
          m_ws_name = _ws_name;
          m_camera = std::unique_ptr<camera_t>(new camera_t());
          m_camera->reset_camera();
          m_gizmo = std::unique_ptr<gizmo_t>(new gizmo_t());
        }

        std::optional<size_t>  get_selected_idx();
        ws_item_t *get_selected();
        std::shared_ptr<ws_item_t> get_selected_sp();

        bool set_selected_item(const size_t sel_idx, bool emit_signal = true);
        void unselect_all(bool emit_signal = true);
        void toggle_edit_mode();
        void ws_changed();
        void reset_camera();
        void set_best_view();
        void render();
        void render_overlay(QPainter &painter);
        void mouse_click(const float mouse_x, const float mouse_y);
        void add_item_to_ws(const std::shared_ptr<ws_item_t> &item_to_add);
        //void dialog_add_geom_from_file(qc_file_fmt file_format);

        void clear_connected_items(std::shared_ptr<ws_item_t> item_to_delete);
        void save_ws_to_json(const std::string filename);
        void load_ws_from_json(const std::string filename);
        void update(float delta_time);
        void set_edit_type(const ws_edit_t new_edit_type);

    };

    struct ws_item_factory {

        static std::shared_ptr<ws_item_t> create_object(const std::string &obj_type);

    };

  }

}

#endif
