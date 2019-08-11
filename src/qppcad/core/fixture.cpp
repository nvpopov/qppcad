#include <qppcad/core/fixture.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void fixture_info_t::load_from_file(const std::string &file_name) {

  std::ifstream ifile(file_name);
  json data;

  try {

    data = json::parse(ifile);
    auto fixutre_name = data.find("fixture_name");
    if (fixutre_name == data.end()) return;
    m_fxt_name = fixutre_name.value();

    auto fixture_type = data.find("fixture_type");
    if (fixture_type == data.end()) return;

    bool fixture_type_is_set{false};
    std::string fixture_type_value = fixture_type.value();

    // detect whether fixture is ws_item_tool
    if (fixture_type_value.find("ws_item_tool") != std::string::npos) {

        m_fxt_type = fixture_type_e::ft_ws_item_tool;
        fixture_type_is_set = true;

        auto fixture_tool_grp = data.find("tool_group");
        m_tool_group = fixture_tool_grp != data.end() ? fixture_tool_grp.value() : "Generic";

        m_initialized = true;

      }

    if (!fixture_type_is_set) return;

  } catch (json::parse_error &e) {
    std::cerr << e.what() << " " << std::endl;
  }

}
