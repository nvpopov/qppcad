#include <qppcad/python/python_comp_chem.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>

using namespace qpp;
using namespace qpp::cad;

void cp2k_helper_t::save_neb_data(std::shared_ptr<ws_item_t> g_start,
                                  std::shared_ptr<ws_item_t> g_end,
                                  std::string path) {

}

std::string cp2k_helper_t::gen_fixed_atoms_section(py::list &fixed_atoms) {

  std::string ret;
  ret  = "&FIXED_ATOMS\nCOMPONENTS_TO_FIX XYZ\n";

  const auto items_per_section = 14;
  size_t num_sections = fixed_atoms.size() / items_per_section;

  for (size_t sec = 0; sec <= num_sections; sec++) {
      ret += "LIST ";
      for (size_t itm_c = 0; itm_c < items_per_section; itm_c++) {
          size_t idx_in_list = sec * items_per_section + itm_c;
          if (idx_in_list < fixed_atoms.size()) {
              auto value = py::cast<int>(fixed_atoms[idx_in_list]);
              ret += " " + std::to_string(value);
            }
        }
      ret += "\n";
    }

  ret += "&END FIXED_ATOMS\n";
  return ret;

}

