#ifndef QPP_CAD_PYTHON_MANAGER_H
#define QPP_CAD_PYTHON_MANAGER_H

#include <qppcad/qppcad.hpp>

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

#include <QStringList>
#include <QString>
#include <QList>

namespace py = pybind11;

namespace qpp {

  namespace cad {

    class python_manager_t {
      public:
        QList<QString> m_commands;
        py::object f_comp;
        py::scoped_interpreter guard{};
        bool execute(std::string command);
        std::string m_output_buffer;
        void get_completion_list(QString text, QStringList &sl);
        python_manager_t();
    };

  }

}


#endif
