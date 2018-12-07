#include <qppcad/python_manager.hpp>

using namespace qpp;
using namespace qpp::cad;

bool python_manager_t::execute(std::string command) {
  m_output_buffer.clear();

  bool ret_status{true};

  try {
    py::exec(command, py::globals());
  } catch (py::error_already_set &err) {
    m_output_buffer+= err.what();
    m_output_buffer+= "\n";
    ret_status = true;
  }

  return ret_status;
}

python_manager_t::python_manager_t() {
  py::exec("from core import *\n"
           "from sq import *\n"
           "import sys\n", py::globals());
  py::exec("sys.stdout = output_redirector()", py::globals());
  //py::exec("print(2+2)");
}
