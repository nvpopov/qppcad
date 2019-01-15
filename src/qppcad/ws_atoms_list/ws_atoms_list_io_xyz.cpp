#include <qppcad/ws_atoms_list/ws_atoms_list_io_xyz.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>

using namespace qpp;
using namespace qpp::cad;

bool ws_atoms_list_io_xyz_t::can_save() {
  return true;
}

bool ws_atoms_list_io_xyz_t::can_load() {
  return true;
}

void ws_atoms_list_io_xyz_t::load_from_stream(std::basic_ostream<char, TRAITS> &stream,
                                              ws_item_t *_item) {
  if (_item->get_type() != ws_atoms_list_t::get_type_static()) return;
  ws_atoms_list_t *al = _item->cast_as<ws_atoms_list_t>();
}

void ws_atoms_list_io_xyz_t::save_to_stream(std::basic_istream<char, TRAITS> &stream,
                                            ws_item_t *_item) {
  if (_item->get_type() != ws_atoms_list_t::get_type_static()) return;
   ws_atoms_list_t *al = _item->cast_as<ws_atoms_list_t>();
}

bool ws_atoms_list_io_xyz_t::deduce_from_file_name(std::string &file_name) {
  return ((file_name.find(".xyz") != std::string::npos) ||
          (file_name.find(".XYZ") != std::string::npos));
}
