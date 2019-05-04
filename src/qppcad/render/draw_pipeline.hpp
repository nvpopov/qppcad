#ifndef QPPCAD_DRAW_PIPELINE
#define QPPCAD_DRAW_PIPELINE
#include <qppcad/qppcad.hpp>
#include <qppcad/render/mesh.hpp>
#include <qppcad/render/shader_program.hpp>
#include <string>
#include <Eigen/Eigen>
#include <geom/lace3d.hpp>

namespace qpp {

  namespace cad {

    enum draw_pipeline_depth_func {
      depth_equal,
      depth_lequal,
      depth_greater,
      depth_less,
      depth_always,
      depth_disabled,
      depth_enabled
    };

    enum draw_pipeline_cull_func {
      cull_front,
      cull_back,
      cull_enable,
      cull_disable
    };

    struct draw_pipeline_t {

        draw_pipeline_t();

        /// \brief load_pipeline
        void load_pipeline();

        /// \brief unload_pipeline
        void unload_pipeline();

        /// \brief render
        void render();

        void depth_func(draw_pipeline_depth_func _action);
        void cull_func(draw_pipeline_cull_func _action);

        void begin_atom_render (float specular_power, float specular_alpha);
        void render_atom (const vector3<float> &color,
                          const vector3<float> &pos,
                          const float radius);
        void end_atom_render();

        void begin_atom_render_suprematic();
        void render_atom_suprematic(const vector3<float> &color,
                                    const vector3<float> &pos,
                                    const float radius);
        void end_atom_render_suprematic();

        /// \brief begin_render_bond
        void begin_render_bond(float specular_power, float specular_alpha);

        /// \brief render_bond
        /// \param color
        /// \param vBondStart
        /// \param vBondEnd
        /// \param fBondRadius
        void render_bond(const vector3<float> &color,
                         const vector3<float> &bond_start,
                         const vector3<float> &bond_end,
                         const float bond_radius);

        /// \brief end_render_bond
        void end_render_bond();

        void begin_render_2c_bond(float specular_power, float specular_alpha);
        void render_2c_bond(const vector3<float> &color1,
                            const vector3<float> &color2,
                            const vector3<float> &bond_start,
                            const vector3<float> &bond_end,
                            const float bond_radius);
        void end_render_2c_bond();

        void begin_render_2c_bond_suprematic();
        void render_2c_bond_suprematic(const vector3<float> &color1,
                                       const vector3<float> &color2,
                                       const vector3<float> &bond_start,
                                       const vector3<float> &bond_end,
                                       const float bond_radius);
        void end_render_2c_bond_suprematic();


        /// \brief render_molecule
        void render_molecule();

        /// \brief render_cell_3d
        /// \param color
        /// \param a
        /// \param b
        /// \param c
        /// \param shift
        /// \param line_width
        void render_cell_3d(const vector3<float> &color,
                            const vector3<float> &a,
                            const vector3<float> &b,
                            const vector3<float> &c,
                            const vector3<float> &shift);

        /// \brief render_vector
        void render_vector();

        /// \brief render_primitive
        void render_primitive();

        /// \brief begin_render_general_mesh
        void begin_render_general_mesh(shader_program_t *custom_sp = nullptr);

        /// \brief render_general_mesh
        /// \param mesh_pos
        /// \param mesh_scale
        /// \param mesh_rotation
        /// \param mesh_color
        /// \param mesh
        void render_general_mesh(const vector3<float> &mesh_pos,
                                 const vector3<float> &mesh_scale,
                                 const vector3<float> &mesh_rotation,
                                 const vector3<float> &mesh_color,
                                 mesh_t *mesh,
                                 float alpha = 1.0f,
                                 shader_program_t *custom_sp = nullptr);

        /// \brief render_general_mesh
        /// \param model_matrix
        /// \param mesh_color
        /// \param mesh
        void render_general_mesh(const matrix4<float> &model_matrix,
                                 const vector3<float> &mesh_color,
                                 mesh_t *mesh,
                                 float alpha = 1.0f,
                                 shader_program_t *custom_sp = nullptr);

        /// \brief render_cube
        /// \param cube_pos
        /// \param cube_size
        /// \param cube_color
        void render_cube(const vector3<float> &cube_pos,
                         const vector3<float> &cube_size,
                         const vector3<float> &cube_color);

        /// \brief render_cone
        /// \param cone_pos
        /// \param cone_size
        /// \param cone_color
        void render_cone(const vector3<float> &cone_pos,
                         const vector3<float> &cone_size,
                         const vector3<float> &cone_color);

        void render_arrow(const vector3<float> &arrow_start,
                          const vector3<float> &arrow_end,
                          const vector3<float> &arrow_color,
                          const float arrow_body_scale,
                          const float arrow_cap_scale,
                          const float arrow_cap_len,
                          const bool inner_cap = true);

        /// \brief end_render_general_mesh
        void end_render_general_mesh(shader_program_t *custom_sp = nullptr);


        /// \brief begin_render_aabb
        void begin_render_aabb();

        /// \brief render_aabb
        /// \param color
        /// \param box_min
        /// \param box_max
        void render_aabb(const vector3<float> &color,
                         const vector3<float> &box_min,
                         const vector3<float> &box_max);

        /// \brief render_aabb_segmented
        /// \param color
        /// \param box_min
        /// \param box_max
        void render_aabb_segmented(const vector3<float> &color,
                                   const vector3<float> &box_min,
                                   const vector3<float> &box_max);

        /// \brief end_render_aabb
        void end_render_aabb();

        /// \brief begin_render_line
        void begin_render_line();

        /// \brief render_line
        /// \param color
        /// \param line_start
        /// \param line_end
        /// \param line_width
        void render_line(const vector3<float> &color,
                         const vector3<float> &line_start,
                         const vector3<float> &line_end);

        /// \brief end_render_line
        void end_render_line();

        void begin_render_line_styled();
        void render_line_styled (const vector3<float> &color,
                                 const vector3<float> &line_start,
                                 const vector3<float> &line_end);
        void end_render_line_styled();

        void begin_render_line_mesh();
        void render_line_mesh (const vector3<float> &pos,
                               const vector3<float> &color);
        void end_render_line_mesh();

        void render_screen_quad();

    };

  } // namespace qpp::cad

} // namespace qpp


#endif
