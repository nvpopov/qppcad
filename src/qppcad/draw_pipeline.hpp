#ifndef QPP_CAD_DRAW_PIPELINE_
#define QPP_CAD_DRAW_PIPELINE_
#include <string>
#include <Eigen/Eigen>
#include <geom/lace3d.hpp>

namespace qpp {

  class draw_pipeline{
  public:
    draw_pipeline();
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
    void render_cell_3d(const vector3<float> &a,
                        const vector3<float> &b,
                        const vector3<float> &c );
    void render_vector();
    void render_primitive();

    void begin_render_aabb();
    void render_aabb(const vector3<float> &vColor,
                     const vector3<float> &vBoxMin,
                     const vector3<float> &vBoxMax);
    void render_aabb_segmented(const vector3<float> &vColor,
                               const vector3<float> &vBoxMin,
                               const vector3<float> &vBoxMax);
    void end_render_aabb();

    void begin_render_line();
    void render_line(const vector3<float> &color,
                     const vector3<float> &vStart,
                     const vector3<float> &vEnd,
                     const float fLineWidth = 1.0f);
    void end_render_line();
  };

}
#endif
