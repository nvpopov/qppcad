#ifndef QPPCAD_DISPL_PROJ_H
#define QPPCAD_DISPL_PROJ_H

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <qppcad/qppcad.hpp>

namespace py = pybind11;

namespace qpp {

  namespace cad {

    struct displ_proj_record_t {

        vector3<float> m_start_pos;
        vector3<float> m_end_pos;
        std::string m_atom_name;

    };

    struct displ_proj_set_t {

        vector3<float> m_cnt;
        std::vector<displ_proj_record_t> m_recs;

    };

    struct py_displ_proj_reg_helper_t {

        static void reg(py::module &module);

    };

  }

}

#endif
