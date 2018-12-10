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

void python_manager_t::get_completion_list(QString text, QStringList &sl) {
  std::string command = fmt::format("complete_text(\"{}\")", text.toStdString());
  py::list retlist = py::eval(command, py::globals());
  sl.clear();
  for (auto elem : retlist) {
      if (py::isinstance<py::str>(elem))
        sl.append(QString::fromStdString(elem.cast<std::string>()));
    }
}


python_manager_t::python_manager_t() {

  py::exec("from core import *\n"
           "from sq import *\n"
           "import pyqpp \n"
           "import rlcompleter \n"
           "import sys\n", py::globals());

  py::exec("sys.stdout = output_redirector()", py::globals());
  py::exec(R"(
           cm = rlcompleter.Completer()

           def complete_text(text):
            ret_list = []
            for i in range(0,40):
              x = cm.complete(text, i)
              if x is None:
                break
              else:
                ret_list.append(x)
            return ret_list

           )", py::globals());

}
