#ifndef QPPCAD_PYTHON_SQ_H
#define QPPCAD_PYTHON_SQ_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <qppcad/workspace.hpp>

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
//#include <pybind11/eigen.h>
#pragma pop_macro("slots")

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class simple_query {

      public:

        static std::shared_ptr<workspace_manager_t> get_ws_mgr();

        /**
         * @brief set_font_size set python console font size
         * @param new_font_size in pt.
         */
        static void set_font_size(int new_font_size);

        /**
         * @brief get_build_info
         * @return git revision && build date
         */
        static std::tuple<std::string, std::string> get_build_info();

        static void open_file(std::string file_name,
                              bool to_current);
        static void open_file_query(std::string file_name,
                                    std::string file_format,
                                    bool to_current);
        static void make_super_cell(const int sc_a,
                                    const int sc_b,
                                    const int sc_c);

        static void select_ws(int ws_idx);
        static void select_itm(int itm_idx);
        static void sel_cnt(int cnt_idx);
        static void sel_cnt_fn(std::function<bool(float, float, float)> cfn);
        static void sel_cnt_parity();
        static void sel_invert();
        static void sel_cnt_all();
        static void sel_cnt_list(py::list sel_list);
        static void sel_cnt_type(py::str sel_type);
        static void sel_cnt_sphere(vector3<float> sph_center,
                                   float sph_rad);
        static void sel_hemisphere(int coord_idx, bool positive);
        static void unsel_cnt_all();
        static void unsel_cnt(int cnt_idx);
        static void unsel_cnt_list(py::list sel_list);
        static void unsel_cnt_type(py::str sel_type);
        static py::list get_sel();

        static py::list get_tool_groups();
        static py::list get_tool_names();
        static void exec_tool(std::shared_ptr<ws_item_t> ws_item, std::string tool_name);
        static py::str get_type_name();
        static py::int_ get_type_hash();
        static py::list get_connected_items(std::shared_ptr<ws_item_t> ws_item);
        static py::list get_followers(std::shared_ptr<ws_item_t> ws_item);
        static py::str get_leader(std::shared_ptr<ws_item_t> ws_item);
        static py::bool_ is_instance_of_by_hash(size_t _type_hash);
        static py::bool_ is_instance_of_by_type_name(std::string _type_name);
        //static void bopen(std::string _file_name);
        static vector3<float> gizmo_pos();
        static std::tuple<std::string, vector3<float> > get_point_sym_group(float tolerance);

        /**
         * @brief embed_cube create cube_primitive_t based on selected atoms from geom_view_ts
         */
        static void embed_cube();

        static void make_psg_view(float tolerance);
        static void make_traj_highlight(size_t atom_id,
                                        size_t anim_id);

        static std::vector<std::string> get_xgeom_dfn();
        static std::vector<basic_types> get_xgeom_dft();
        static void edit_mode(int mode);
        static void rebond();
        static void translate_selected(float tx,
                                       float ty,
                                       float tz);
        static void set_charge(float charge);
        static void set_ws_bg(vector3<float> bg);
        static std::shared_ptr<workspace_t> cur_ws();

        static void make_cube_p(std::string name,
                                vector3<float> pos,
                                float size_a,
                                float size_b,
                                float size_c);

        static void make_arrow_p(std::string name,
                                 vector3<float> from,
                                 vector3<float> to);

        static void convert_selected_units(spatial_units_e new_unit);

        static void set_sel_color_vec(vector3<float> color);
        static void set_sel_color(float r, float g, float b);

        static void ptable_set_color_by_number(int num,
                                               float r,
                                               float g,
                                               float b);
        static void ptable_set_color_by_name(std::string name,
                                             float r,
                                             float g,
                                             float b);
        static void ptable_set_radius_by_number(int num,
                                                float r);
        static void ptable_set_radius_by_name(std::string name,
                                              float r);

        static vector3<float> ptable_get_color_by_number(int num);
        static vector3<float> ptable_get_color_by_name(std::string name);
        static float ptable_get_radius_by_number(int num);
        static float ptable_get_radius_by_name(std::string name);

        static void camera_move(vector3<float> axis, float magnitude);
        static void camera_rotate_yaw(float magnitude);
        static void camera_rotate_pitch(float magnitude);
        static void camera_zoom(float magnitude);
        static float camera_get_zoom();
        static void camera_mode(int mode);
        static void copy_camera_from_ws(int ws_id);

        static py::list sv_get();
        static void sv_edit(int at,
                            bool status);
        static void sv_edit_list(py::list at_list,
                                 bool status);
        static void sv_edit_all(bool status);

        static py::list cl_get();
        static void set_cl_state(int at,
                                 bool status);
        static void set_cl_text(int at,
                                std::string text);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
