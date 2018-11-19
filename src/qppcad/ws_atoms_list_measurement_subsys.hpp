#ifndef QPP_CAD_WS_ATOMS_LIST_MEASURE_S
#define QPP_CAD_WS_ATOMS_LIST_MEASURE_S

#include <geom/geom.hpp>
//#include <qppcad/app.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    class ws_atoms_list_t;

    template<typename AINT = uint32_t>
    struct measurement_bond_record {

        AINT at1;
        AINT at2;
        index idx1;
        index idx2;
        bool m_show{true};

        measurement_bond_record (const AINT _at1, const AINT _a2,
                                 const index _idx1, const index _idx2) :
          at1(_at1), at2(_a2), idx1(_idx1), idx2(_idx2) {}

    };

    class ws_atoms_list_measurement_subsys_t {

      private:

        ws_atoms_list_t* p_owner;
        std::vector<measurement_bond_record<uint32_t> > m_records;

      public:

        ws_atoms_list_measurement_subsys_t (ws_atoms_list_t &_p_owner);

        float dist (const size_t idx);

        void add_bond_measurement (const uint32_t atm1, const uint32_t atm2,
                                   const index idx1, const index idx2);

        void remove_bond_measurement (const size_t measure_idx);

        std::optional<size_t> is_bond_measurement_exist (const uint32_t atm1, const uint32_t atm2,
                                                         const index idx1, const index idx2);

        void render ();

//        void render_overlay ();

//        void render_ui_obj_inst ();

//        void render_ui_context (); // end render_ui_context
    };

  }

}

#endif
