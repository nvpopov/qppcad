#ifndef QPP_WORKSPACE_H
#define QPP_WORKSPACE_H



#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/geom.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/camera.hpp>
#include <qppcad/ws_item.hpp>
#include <qppcad/ws_atoms_list.hpp>
#include <qppcad/gizmo.hpp>
#include <qppcad/delegates.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace qpp{

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
     uint8_t m_current_workspace_id;
  public:

    vector<shared_ptr<workspace_t> > m_ws;

    workspace_manager_t(app_state_t *_astate);

    shared_ptr<workspace_t> get_current();
    optional<uint8_t> get_current_id();

    bool set_current(const uint8_t ws_index);

    template<int I>
    void force_set_current(){set_current(I);}

    bool has_wss(){return m_ws.size()>0;}
    void init_default();
    void render_current_workspace();
    void mouse_click();
    void add_workspace(const shared_ptr<workspace_t> &ws_to_add);
    void query_import_file_as_new_workspace(qc_file_format file_format);
  };

  ///
  /// \brief The workspace_t class
  ///
  class workspace_t : public std::enable_shared_from_this<workspace_t> {
  public:
    ws_edit_type                   m_edit_type;
    vector<shared_ptr<ws_item_t> > m_ws_items;
    string                         m_ws_name;
    unique_ptr<camera_t>           m_camera;
    ray_t<float>                   m_ray_debug;
    bool                           m_first_render;
    vector<string>                 m_ws_names_c;
    unique_ptr<gizmo_t>            m_gizmo;

    workspace_t(string _ws_name = "default"){
      m_ws_name = _ws_name;
      m_first_render = true;
      m_camera = unique_ptr<camera_t>(new camera_t());
      m_camera->reset_camera();
      m_edit_type = ws_edit_type::EDIT_WS_ITEM;
      m_gizmo = unique_ptr<gizmo_t>(new gizmo_t());
    }

    int16_t get_selected_item();
    ws_item_t *get_selected();
    void set_selected_item(const int16_t sel_idx);
    void unselect_all();
    void toggle_edit_mode();
    void workspace_changed();
    void reset_camera();
    void set_best_view();
    void render();
    void mouse_click(const double mouse_x, const double mouse_y);
    void add_item_to_workspace(const shared_ptr<ws_item_t> &item_to_add);
    void dialog_add_geom_from_file(qc_file_format file_format);
    void update(float delta_time);
  };


}

#endif
