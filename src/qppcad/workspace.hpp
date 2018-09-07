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

  enum ws_edit_type {
    EDIT_WS_ITEM = 0,
    EDIT_WS_ITEM_CONTENT = 1
  };

  ///
  /// \brief The workspace_manager_t class
  ///
  class workspace_manager_t {
  private:
     uint8_t current_workspace_id;
  public:


    std::vector<workspace_t*> ws;
    workspace_manager_t(){ current_workspace_id = 0;}

    workspace_t* get_current_workspace();
    uint8_t get_current_workspace_id();

    bool set_current_workspace(const uint8_t ws_index);

    bool has_wss(){return ws.size()>0;}
    void init_default_workspace();
    void render_current_workspace();
    void mouse_click();
    void query_import_file_as_new_workspace(qc_file_format file_format);
  };

  ///
  /// \brief The workspace_t class
  ///
  class workspace_t {
  public:
    ws_edit_type             cur_edit_type;
    std::vector<ws_item_t*>  ws_items;
    std::string              ws_name;
    camera_t*                camera;
    ray_t<float>               ray_debug;
    bool                     first_render;
    std::vector<std::string> ws_names_c;
    gizmo_t*                 gizmo;

    workspace_t(std::string _ws_name = "default"){
      ws_name = _ws_name;
      first_render = true;
      camera = new camera_t();
      camera->reset_camera();
      cur_edit_type = ws_edit_type::EDIT_WS_ITEM;
      gizmo = new gizmo_t();
    }

    int16_t get_selected_item();
    ws_item_t *get_selected();
    void set_selected_item(const int16_t sel_idx);
    void unselect_all();

    void workspace_changed();
    void reset_camera();
    void set_best_view();
    void render();
    void mouse_click(const double mouse_x, const double mouse_y);
    void add_item_to_workspace(ws_item_t *item_to_add);
    void update(float delta_time);
  };


}

#endif
