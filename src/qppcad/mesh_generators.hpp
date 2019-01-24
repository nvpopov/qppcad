#ifndef QPPCAD_MESH_GENERATORS
#define QPPCAD_MESH_GENERATORS

#include <qppcad/qppcad.hpp>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <geom/lace3d.hpp>
#include <qppcad/mesh.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace qpp {

  namespace cad {

    struct mesh_generators{

        static mesh_t *plane_zl ();

        static mesh_t *quad_zup ();

        static mesh_t *quad ();
        static mesh_t *sphere(const int lat_bands, const int long_bands);
        static mesh_t *cylinder_whole(const int num_phi, const int num_z);
        static mesh_t *cylinder_mk2 (const int num_segment_height, const int num_segment_base,
                                     const float radius, const float height);
        static mesh_t *unit_line ();
        static mesh_t *cone (const float radius = 1.0f, const float height = 1.0f,
                            const uint8_t num_segment_height = 1,
                            const uint8_t num_segment_base   = 10);
        static mesh_t *unit_cube ();

        static mesh_t *xz_plane(const int n_x,const float dx,
                                const int n_z, const float dz);
        static mesh_t *cross_line_atom ();

    };

  }

}

#endif
