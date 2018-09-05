#ifndef QPP_CAD_DRAW_PIPELINE_
#define QPP_CAD_DRAW_PIPELINE_
#include <string>
#include <Eigen/Eigen>
#include <geom/lace3d.hpp>

namespace qpp {

  class draw_pipeline_t{
  public:
    draw_pipeline_t();
    void load_pipeline();
    void unload_pipeline();
    void render();

    void begin_atom_render();
    void render_atom(const vector3<float> &color,
                     const vector3<float> &pos,
                     const float radius);
    void end_atom_render();

    void begin_render_bond();
    void render_bond(const vector3<float> &color,
                     const vector3<float> &vBondStart,
                     const vector3<float> &vBondEnd,
                     const float fBondRadius);
    void end_render_bond();

    void render_molecule();
    void render_cell_3d(const vector3<float> &color,
                        const vector3<float> &a,
                        const vector3<float> &b,
                        const vector3<float> &c,
                        const vector3<float> &shift,
                        const float line_width);
    void render_vector();
    void render_primitive();

    void begin_render_general_mesh();
    void render_general_mesh(const vector3<float> &mesh_pos,
                             const vector3<float> &mesh_scale,
                             const vector3<float> &mesh_color);
    void render_cube(const vector3<float> &cube_pos,
                     const vector3<float> &cube_size,
                     const vector3<float> &cube_color);
    void render_cone(const vector3<float> &cone_pos,
                     const vector3<float> &cone_size,
                     const vector3<float> &cone_color);
    void end_render_general_mesh();

    void begin_render_aabb();
    void render_aabb(const vector3<float> &color,
                     const vector3<float> &box_min,
                     const vector3<float> &box_max);
    void render_aabb_segmented(const vector3<float> &color,
                               const vector3<float> &box_min,
                               const vector3<float> &box_max);
    void end_render_aabb();

    void begin_render_line();
    void render_line(const vector3<float> &color,
                     const vector3<float> &line_start,
                     const vector3<float> &line_end,
                     const float line_width = 1.0f);
    void end_render_line();
  };

}
#endif
