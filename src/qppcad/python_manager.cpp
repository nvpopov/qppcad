#include <qppcad/python_manager.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

python_manager_t::python_manager_t() {
  core = py::module::import("core");
  sys = py::module::import("sys");
  global = py::dict(py::module::import("__main__").attr("__dict__"));
  py::exec("import core, sys", py::globals());
  py::exec("sys.stdout = core.output_redirector()", py::globals());
  py::print("@@32edqwef123r23");
}
