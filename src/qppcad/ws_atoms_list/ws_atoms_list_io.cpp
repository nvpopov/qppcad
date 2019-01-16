#include <qppcad/ws_atoms_list/ws_atoms_list_io.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>

using namespace qpp;
using namespace qpp::cad;

bool ws_atoms_list_io_xyz_t::can_save() {
  return true;
}

bool ws_atoms_list_io_xyz_t::can_load() {
  return true;
}

void ws_atoms_list_io_xyz_t::load_from_stream_ex(std::basic_istream<char, TRAITS> &stream,
                                                 ws_atoms_list_t *_item) {
  //
}

void ws_atoms_list_io_xyz_t::save_to_stream_ex(std::basic_ostream<char, TRAITS> &stream,
                                               ws_atoms_list_t *_item) {

}



