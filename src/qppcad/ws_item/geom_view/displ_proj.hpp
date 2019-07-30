#ifndef QPPCAD_DISPL_PROJ_H
#define QPPCAD_DISPL_PROJ_H

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#pragma pop_macro("slots")

#include <qppcad/qppcad.hpp>

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class geom_view_t;

    struct displ_proj_record_t {

        vector3<float> m_start_pos;
        vector3<float> m_end_pos;
        std::string m_atom_name;

        displ_proj_record_t(vector3<float> start_pos,
                            vector3<float> end_pos,
                            const std::string &aname) :
          m_start_pos(start_pos), m_end_pos(end_pos), m_atom_name(aname) {}

        vector3<float> get_displ();

    };

    struct displ_proj_set_t {

        vector3<float> m_cnt;
        std::vector<displ_proj_record_t> m_recs;
        displ_proj_set_t(std::shared_ptr<geom_view_t> gs,
                         std::shared_ptr<geom_view_t> ge,
                         std::vector<size_t> atlist);
    };

    struct py_displ_proj_reg_helper_t {

        static void reg(py::module &module);

    };

  }

}

#endif
