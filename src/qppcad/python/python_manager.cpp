#include <qppcad/python/python_manager.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

bool python_manager_t::execute(std::string command) {

  app_state_t *astate = app_state_t::get_inst();

  m_output_buffer.clear();

  bool ret_status{true};

  try {

    bool comm_has_equality = command.find("=") != std::string::npos;
    bool comm_is_multiline = std::count(command.begin(), command.end(), '\n') > 0;
    //    bool comm_has_double_equality = command.find("==") != std::string::npos;
    bool comm_has_statement =
        command.find("for") != std::string::npos || command.find("import") != std::string::npos;
    bool comm_is_empty = !(command.length() > 0);
    bool comm_is_multi = command.find(";") != std::string::npos;

    if(!comm_is_empty) {
        if (!comm_has_equality && !comm_is_multiline && !comm_has_statement && !comm_is_multi) {
            py::object res = py::eval(command, py::globals());

            if (py::isinstance<py::int_>(res) || py::isinstance<py::float_>(res) ||
                py::isinstance<py::str>(res) || py::isinstance<py::list>(res) ||
                py::isinstance<py::dict>(res) || py::isinstance<py::bool_>(res) ||
                py::isinstance<vector3<float>>(res) || py::isinstance<matrix3<float>>(res) ||
                py::hasattr(res, "__str__")) {
                py::print(res);
              }
          } else {
            py::eval<py::eval_statements>(command, py::globals());
          }
      }

  } catch (py::error_already_set &err) {

    m_output_buffer+= err.what();
    astate->make_viewport_dirty();
    ret_status = true;

  }

  astate->make_viewport_dirty();
  return ret_status;

}

void python_manager_t::get_completion_list(QString text, QStringList &sl) {

  std::string command = fmt::format("complete_text(\"{}\")", text.toStdString());
  py::list retlist;

  try {
    retlist = py::eval(command, py::globals());
  } catch (py::error_already_set &err) {

  }

  sl.clear();
  for (auto elem : retlist) {
      if (py::isinstance<py::str>(elem))
        sl.append(QString::fromStdString(elem.cast<std::string>()));
    }

}


python_manager_t::python_manager_t() {

  std::string bootstrap =
    #include <python_files/bootstrap.py>
      ;
  py::exec(bootstrap, py::globals());

}
