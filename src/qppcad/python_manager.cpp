#include <qppcad/python_manager.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

bool python_manager_t::execute(std::string command) {

  app_state_t *astate = app_state_t::get_inst();

  m_output_buffer.clear();

  bool ret_status{true};

  try {
    py::exec(command, py::globals());
  } catch (py::error_already_set &err) {
    m_output_buffer+= err.what();
    m_output_buffer+= "\n";
    astate->make_viewport_dirty();
    ret_status = true;
  }

  astate->make_viewport_dirty();
  return ret_status;
}

python_manager_t::python_manager_t() {

  py::exec("from core import *\n"
           "from sq import *\n"
           "import pyqpp \n"
           "import rlcompleter \n"
           "import sys\n", py::globals());

  py::exec("sys.stdout = output_redirector()", py::globals());
  //py::exec("completer = rlcompleter.Completer()", py::globals());

  //f_comp = py::globals().attr("__main__").attr("completer").attr("complete");
  //py::print(py::globals());
  //py::print(rlcompl.attr("complete")("pyqpp.", 0));
  //py::exec("print(2+2)");

}
