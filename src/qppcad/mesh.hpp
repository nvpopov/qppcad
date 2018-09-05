#ifndef QPP_MESH_H
#define QPP_MESH_H

#include <qppcad/qppcad.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <geom/lace3d.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace qpp{

  ///
  /// \brief The mesh_rt enum
  ///
  enum mesh_rt {
    mesh_rt_triangles,
    mesh_rt_lines
  };

  ///
  /// \brief The mesh_t class
  ///
  class mesh_t{
    public:

      /// Raw vertex data
      std::vector<float> vertecies;
      std::vector<int>   indices;
      std::vector<float> normals;

      uint32_t num_primitives;
      uint32_t num_indices;

      /// OpenGL buffer handles, todo: move it to template driven abonimation
      GLuint vio;
      GLuint vbo;
      GLuint vao;
      GLuint nbo;
      GLenum mesh_rt;

      mesh_t();

      ///
      /// \brief generate_sphere_mesh
      /// \param lat_bands
      /// \param long_bands
      /// \return
      ///
      static mesh_t *generate_sphere_mesh(const int lat_bands, const int long_bands);

      ///
      /// \brief generate_cylinder_whole
      /// \param num_phi
      /// \param num_z
      /// \return
      ///
      static mesh_t *generate_cylinder_whole(const int num_phi, const int num_z);

      ///
      /// \brief generate_unit_line
      /// \return
      ///
      static mesh_t *generate_unit_line();

      static mesh_t *generate_cone_mesh(const float radius = 1.0f,
                                        const float height = 1.0f,
                                        const uint8_t num_segment_height = 1,
                                        const uint8_t num_segment_base   = 10);
      ///
      /// \brief generate_unit_cube
      /// \return
      ///
      static mesh_t *generate_unit_cube();

      ///
      /// \brief generate_xz_plane
      /// \param n_x
      /// \param dx
      /// \param n_z
      /// \param dz
      /// \return
      ///
      static mesh_t *generate_xz_plane(const int n_x,const float dx, const int n_z, const float dz);

      ///
      /// \brief render
      ///
      void render();

      ///
      /// \brief bind_data
      ///
      void bind_data();
  };

  template<typename REAL = float>
  void dump_vector3_to_vector(std::vector<REAL> &invec,
                              const vector3<REAL> _v){
    for (uint8_t i = 0; i < 3; i++) invec.push_back(_v(i));
  }

}
#endif
