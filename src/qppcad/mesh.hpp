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
  class mesh{
  public:

    /// Raw vertex data
    std::vector<float> vertexData;
    std::vector<int> indicesData;
    std::vector<float> normalData;

    unsigned int num_vertices;
    unsigned int num_indices;

    /// OpenGL buffer handles
    GLuint vio;
    GLuint vbo;
    GLuint vao;
    GLuint nbo;
    GLenum mesh_rt;

    mesh();
    static mesh *generate_sphere_mesh(const int latBands, const int longBands);
    static mesh *generate_cylinder_whole(const int numPhi, const int numZ);
    static mesh *generate_unit_line();
    static mesh *generate_xz_plane(const int iNumX,
                                   const float fDeltaX,
                                   const int iNumZ,
                                   const float fDeltaZ);
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
