#include <qppcad/python/python_console_output_redirectors.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void python_std_output_redirector_t::write(pybind11::str data) {
  app_state_t::get_inst()->py_manager->m_output_buffer += data.cast<std::string>();
}
