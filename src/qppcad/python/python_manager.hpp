#ifndef QPPCAD_PYTHON_MANAGER_H
#define QPPCAD_PYTHON_MANAGER_H

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
        std::string m_output_buffer;

        python_manager_t();
        bool execute(std::string command);
        void get_completion_list(QString text, QStringList &sl);


    };

  } // namespace qpp::cad

} // namespace qpp

#endif
