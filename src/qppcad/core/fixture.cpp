#include <qppcad/core/fixture.hpp>
#include <qppcad/core/app_state.hpp>
#include <QFileInfo>

using namespace qpp;
using namespace qpp::cad;

void fixture_info_t::load_from_file(const std::string &file_name, const std::string &dir_name) {

  std::ifstream ifile(file_name);
  json data;

  try {

    data = json::parse(ifile);
    auto fxt_name = data.find("fixture_name");
    if (fxt_name == data.end()) return;
    m_fxt_name = fxt_name.value();

    auto fxt_type = data.find("fixture_type");
    if (fxt_type == data.end()) return;

    bool fxt_type_is_set{false};
    std::string fxt_type_value = fxt_type.value();

    bool fxt_file_is_exists{false};

    // detect whether fixture is ws_item_tool
    if (fxt_type_value.find("ws_item_tool") != std::string::npos) {

        m_fxt_type = fixture_type_e::ft_ws_item_tool;
        fxt_type_is_set = true;

        auto fxt_tool_grp = data.find("tool_group");
        m_tool_group = fxt_tool_grp != data.end() ? fxt_tool_grp.value() : "Generic tools";

        auto fxt_script_file = data.find("fixture_file");

        if (fxt_script_file != data.end()) {

            std::string fix_script_file_name_val = fxt_script_file.value();
            std::string fix_script_file_name = fmt::format("{}/{}",
                                                           dir_name,
                                                           fix_script_file_name_val);

            QFileInfo fix_script_fi(QString::fromStdString(fix_script_file_name));
            if (fix_script_fi.exists() && fix_script_fi.isFile()) {

                m_fxt_script_fn = fix_script_file_name;
                fxt_file_is_exists = true;

              }

          } // if (fixture_script_file != data.end())

        m_initialized = fxt_type_is_set && fxt_file_is_exists;

      }

    if (!fxt_type_is_set) return;

  } catch (json::parse_error &e) {
    std::cerr << e.what() << " " << std::endl;
  }

}
