#ifndef QPPCAD_FIXTURE
#define QPPCAD_FIXTURE

#include <qppcad/core/qppcad.hpp>

namespace qpp {

  namespace cad {

    class app_state_t;

    enum fixture_type_e : int {
      ft_none,
      ft_ws_item_tool,
      ft_file_format
    };

    struct fixture_info_t {

        std::string m_fixture_name;
        std::string m_fixure_path;
        std::string m_tool_group;
        fixture_type_e m_fixture_type{fixture_type_e::ft_none};
        bool m_initialized{false};
        void load_from_file(const std::string &file_name);

    };

  } // namespace cad

} // namespace qpp

#endif
