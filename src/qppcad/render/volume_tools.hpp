#ifndef QPPCAD_MARCHING_CUBES_H
#define QPPCAD_MARCHING_CUBES_H

#include <qppcad/qppcad.hpp>
#include <qppcad/render/mesh.hpp>
#include <io/cube.hpp>
#include <symm/cell.hpp>

namespace qpp {

  namespace cad {

    class volume_helper {

      public:

        //based on http://paulbourke.net/geometry/polygonise/

        static void polygonise_vol_mc(mesh_t &mesh,
                                      scalar_volume_t<float> &volume,
                                      float isolevel,
                                      int steps,
                                      int addr_mode = 0);

        static vector3<float> get_vertex_normal_from_cube(const int ix,
                                                          const int iy,
                                                          const int iz,
                                                          scalar_volume_t<float> &volume,
                                                          int &addr_mode);

        static float get_value_from_cube_interpolated(vector3<float> &pos,
                                                      scalar_volume_t<float> &volume,
                                                      int &addr_mode);

        static vector3<float> vertex_interp(float isolevel,
                                            vector3<float> &p1, vector3<float> &p2,
                                            float val_p1, float val_p2);
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
