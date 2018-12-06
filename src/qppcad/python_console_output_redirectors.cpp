#include <qppcad/python_console_output_redirectors.hpp>

using namespace qpp;
using namespace qpp::cad;

void python_std_output_redirector_t::write(pybind11::str data) {
  std::cout << data.cast<std::string>();
}
