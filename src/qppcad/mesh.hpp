#ifndef QPP_MESH_H
#define QPP_MESH_H
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <io/strfun.hpp>
#include <geom/lace3d.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace qpp{

  /// mesh render type
  enum mesh_rt {
    mesh_rt_triangles,
    mesh_rt_lines
  };

  /// Mesh consist of vertexes, indices and normals
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
    static mesh_t *generate_sphere_mesh(const int lat_bands, const int long_bands);
    static mesh_t *generate_cylinder_whole(const int num_phi, const int num_z);
    static mesh_t *generate_unit_line();
    static mesh_t *generate_cone();
    static mesh_t *generate_unit_cube();
    static mesh_t *generate_xz_plane(const int n_x,const float dx, const int n_z, const float dz);
    void render();
    void bind_data();
  };

  template<typename REAL>
  void dump_vector3_to_vector(std::vector<REAL> &invec,
                              const vector3<REAL> _v){
    for (int i = 0; i < 3; i++) invec.push_back(_v(i));
  }

}
#endif
