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
        vector3<REAL> m_rotation{0.0f, 0.0f, 0.0f};

        REAL d{0.0f};
        REAL m_size{10.0f};

        int m_h{0};
        int m_k{0};
        int m_l{0};

        void update(geometry<REAL, periodic_cell<REAL> > *geom, int _h, int _k, int _l) {

          m_h = _h;
          m_k = _k;
          m_l = _l;

          vector3<REAL> pp1 = geom->cell.v[0] / m_h;
          vector3<REAL> pp2 = geom->cell.v[1] / m_k;
          vector3<REAL> pp3 = geom->cell.v[2] / m_l;

          m_size = std::max(geom->cell.v[0].norm(),
              std::max(geom->cell.v[1].norm(), geom->cell.v[2].norm()));

          //  norm = cross(p1-p0,p2-p0);
          //    norm.normalize();
          //    d = -dot(norm,p0);

          m_center = pp1;

          m_normal = ((pp1 - pp2).cross(pp2 - pp3)).normalized();
          d = - (pp1.dot(m_normal));

          m_center = (pp1 + pp2 + pp3) / REAL(3);

          m_rotation[0] = qpp::pi+std::acos(m_normal.dot(vector3<float>::UnitZ()));
          m_rotation[1] = std::acos(m_normal.dot(vector3<float>::UnitY()));
          m_rotation[2] = std::acos(m_normal.dot(vector3<float>::UnitX()));

        }

        lattice_plane_record_t(geometry<REAL, periodic_cell<REAL> > *geom, int _h, int _k, int _l) {

          update(geom, _h, _k, _l);

        }

    };

    class ws_atoms_list_t;

    class ws_atoms_list_lat_planes_subsys_t {
      private:
        ws_atoms_list_t* p_owner;
        std::vector<lattice_plane_record_t<float> > m_planes;
      public:
        ws_atoms_list_lat_planes_subsys_t (ws_atoms_list_t &_p_owner);
        void add_plane (int _h = 1, int _k = -1, int _l = -1);
        void render ();
        void render_ui_obj_insp ();
    };

  }

}

#endif
