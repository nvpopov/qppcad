#ifndef QPP_WORKSPACE_H
#define QPP_WORKSPACE_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/gizmo.hpp>
#include <qppcad/delegates.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stl.h>

namespace qpp::cad{

  class workspace_t;
  class ws_item_t;

  class app_state_t;

  enum ws_edit_type {
    EDIT_WS_ITEM = 0,
    EDIT_WS_ITEM_CONTENT = 1
  };

  ///
  /// \brief The workspace_manager_t class
  ///
  class workspace_manager_t {
  private:
     size_t m_current_workspace_id;
  public:

    vector<shared_ptr<workspace_t> > m_ws;

    workspace_manager_t(app_state_t *_astate);

    shared_ptr<workspace_t> get_current();
    optional<size_t> get_current_id();

    bool set_current(const size_t ws_index);

    template<int I>
    void force_set_current(){set_current(I);}

    bool has_wss(){return m_ws.size()>0;}
    void init_default();
    void render_current_workspace();
    void mouse_click();
    void add_workspace(const shared_ptr<workspace_t> &ws_to_add);
    void query_import_file_as_new_workspace(qc_file_format file_format);
    void query_create_new_workspace(const bool switch_to_new_workspace = true);
    void dialog_load_workspace();
    void dialog_save_workspace(const size_t ws_idx, const bool force_save_as);
    void dialog_save_current_workspace(const bool force_save_as);
    void update_window_title();

  };

  ///
  /// \brief The workspace_t class
  ///
  class workspace_t : public std::enable_shared_from_this<workspace_t> {
  public:
    string                         m_fs_path{""};
    ws_edit_type                   m_edit_type;
    vector<shared_ptr<ws_item_t> > m_ws_items;
    string                         m_ws_name;
    unique_ptr<camera_t>           m_camera;
    ray_t<float>                   m_ray_debug;
    bool                           m_first_render;
    vector<string>                 m_ws_names_c;
    unique_ptr<gizmo_t>            m_gizmo;
    vector3<float>                 m_background_color{0.85f, 0.85f, 0.85f};

    workspace_t(string _ws_name = "default"){
      m_ws_name = _ws_name;
      m_first_render = true;
      m_camera = unique_ptr<camera_t>(new camera_t());
      m_camera->reset_camera();
      m_edit_type = ws_edit_type::EDIT_WS_ITEM;
      m_gizmo = unique_ptr<gizmo_t>(new gizmo_t());
    }

    optional<size_t>  get_selected_item();
    ws_item_t *get_selected();

    void set_selected_item(const size_t sel_idx);
    void unselect_all();
    void toggle_edit_mode();
    void workspace_changed();
    void reset_camera();
    void set_best_view();
    void render();
    void mouse_click(const float mouse_x, const float mouse_y);
    void add_item_to_workspace(const shared_ptr<ws_item_t> &item_to_add);
    void dialog_add_geom_from_file(qc_file_format file_format);

    void save_workspace_to_json(const string filename);
    void load_workspace_from_json(const string filename);
    void update(float delta_time);
  };


  struct ws_item_factory {
      static shared_ptr<ws_item_t> create_object(const string &obj_type);
  };
}

#endif
