#ifndef QPPCAD_FIXTURE
#define QPPCAD_FIXTURE

#include <qppcad/qppcad.hpp>

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
        fixture_type_e m_fixture_type{fixture_type_e::ft_none};
        bool m_initialized{false};

    };

    struct fixture_loader_helper {

        static void load_fixtures_from_file(app_state_t *astate);

    };

  } // namespace cad

} // namespace qpp

#endif
