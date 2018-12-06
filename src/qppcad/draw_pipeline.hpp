#ifndef QPP_CAD_DRAW_PIPELINE
#define QPP_CAD_DRAW_PIPELINE
#include <qppcad/qppcad.hpp>
#include <qppcad/mesh.hpp>
#include <string>
#include <Eigen/Eigen>
#include <geom/lace3d.hpp>

namespace qpp {

  namespace cad {

    class draw_pipeline_t{

      public:

        draw_pipeline_t ();

        /// \brief load_pipeline
        void load_pipeline ();

        /// \brief unload_pipeline
        void unload_pipeline ();

        /// \brief render
        void render ();

        /// \brief begin_atom_render
        void begin_atom_render (float specular_power, float specular_alpha);

        /// \brief render_atom
        /// \param color
        /// \param pos
        /// \param radius
        void render_atom (const vector3<float> &color,
                          const vector3<float> &pos,
                          const float radius);

        /// \brief end_atom_render
        void end_atom_render ();

        /// \brief begin_render_bond
        void begin_render_bond (float specular_power, float specular_alpha);

        /// \brief render_bond
        /// \param color
        /// \param vBondStart
        /// \param vBondEnd
        /// \param fBondRadius
        void render_bond (const vector3<float> &color,
                          const vector3<float> &bond_start,
                          const vector3<float> &bond_end,
                          const float fBondRadius);

        /// \brief end_render_bond
        void end_render_bond ();

        /// \brief render_molecule
        void render_molecule ();

        /// \brief render_cell_3d
        /// \param color
        /// \param a
        /// \param b
        /// \param c
        /// \param shift
        /// \param line_width
        void render_cell_3d (const vector3<float> &color,
                             const vector3<float> &a,
                             const vector3<float> &b,
                             const vector3<float> &c,
                             const vector3<float> &shift,
                             const float line_width);

        /// \brief render_vector
        void render_vector ();

        /// \brief render_primitive
        void render_primitive ();

        /// \brief begin_render_general_mesh
        void begin_render_general_mesh ();

        /// \brief render_general_mesh
        /// \param mesh_pos
        /// \param mesh_scale
        /// \param mesh_rotation
        /// \param mesh_color
        /// \param mesh
        void render_general_mesh (const vector3<float> &mesh_pos,
                                  const vector3<float> &mesh_scale,
                                  const vector3<float> &mesh_rotation,
                                  const vector3<float> &mesh_color,
                                  mesh_t *mesh);

        /// \brief render_general_mesh
        /// \param model_matrix
        /// \param mesh_color
        /// \param mesh
        void render_general_mesh (const matrix4<float> &model_matrix,
                                  const vector3<float> &mesh_color,
                                  mesh_t *mesh);

        /// \brief render_cube
        /// \param cube_pos
        /// \param cube_size
        /// \param cube_color
        void render_cube (const vector3<float> &cube_pos,
                          const vector3<float> &cube_size,
                          const vector3<float> &cube_color);

        /// \brief render_cone
        /// \param cone_pos
        /// \param cone_size
        /// \param cone_color
        void render_cone (const vector3<float> &cone_pos,
                          const vector3<float> &cone_size,
                          const vector3<float> &cone_color);

        /// \brief end_render_general_mesh
        void end_render_general_mesh ();


        /// \brief begin_render_aabb
        void begin_render_aabb ();

        /// \brief render_aabb
        /// \param color
        /// \param box_min
        /// \param box_max
        void render_aabb (const vector3<float> &color,
                          const vector3<float> &box_min,
                          const vector3<float> &box_max);

        /// \brief render_aabb_segmented
        /// \param color
        /// \param box_min
        /// \param box_max
        void render_aabb_segmented (const vector3<float> &color,
                                    const vector3<float> &box_min,
                                    const vector3<float> &box_max);

        /// \brief end_render_aabb
        void end_render_aabb ();

        /// \brief begin_render_line
        void begin_render_line ();

        /// \brief render_line
        /// \param color
        /// \param line_start
        /// \param line_end
        /// \param line_width
        void render_line (const vector3<float> &color,
                          const vector3<float> &line_start,
                          const vector3<float> &line_end,
                          const float line_width = 1.0f);

        /// \brief end_render_line
        void end_render_line();

        void begin_render_line_styled ();
        void render_line_styled (const vector3<float> &color,
                                 const vector3<float> &line_start,
                                 const vector3<float> &line_end,
                                 const float line_width = 1.0f);
        void end_render_line_styled();



        void render_screen_quad();
    };

  }

}
#endif
