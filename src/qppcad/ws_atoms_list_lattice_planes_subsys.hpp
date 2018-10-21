#ifndef QPP_CAD_WS_ATOMS_LIST_LATPL_S
#define QPP_CAD_WS_ATOMS_LIST_LATPL_S

#include <geom/geom.hpp>
#include <qppcad/app.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    template <typename REAL>
    struct lattice_plane_record_t {

        vector3<REAL> m_center{0.0f, 0.0f, 0.0f};
        vector3<REAL> m_normal{0.0f, 1.0f, 0.0f};

        REAL m_size{10.0f};

        int m_h{0};
        int m_k{0};
        int m_l{0};

        REAL a;
        REAL b;
        REAL c;
        REAL d;

        void update(geometry<REAL, periodic_cell<REAL> > *geom, int _h, int _k, int _l);

        lattice_plane_record_t(geometry<REAL, periodic_cell<REAL> > *geom, int _h, int _k, int _l);

    };

    template <typename DATA, typename REAL = float, typename AINT  = uint32_t>
    class ws_atoms_list_lat_planes_subsys_t {
      private:
        DATA* p_owner;
        std::vector<lattice_plane_record_t<REAL> > m_planes;

      public:
        ws_atoms_list_lat_planes_subsys_t (DATA &_p_owner) {
          p_owner = &_p_owner;
        }

        void add_plane (int _h = 1, int _k = -1, int _l = -1);

        void render ();

        void render_ui_obj_insp ();
    };

  }

}

#endif
