#ifndef QPPCAD_FIXTURE
#define QPPCAD_FIXTURE

#include <qppcad/qppcad.hpp>

namespace qpp {

  namespace cad {

    enum fixture_type_e : int {
      ft_none,
      ft_tool,
      ft_file_format
    };

    struct fixture_info_t {
        std::string m_fixture_name;
        std::string m_fixure_path;
        fixture_type_e m_fixture_type{fixture_type_e::ft_none};
    };

  } // namespace cad

} // namespace qpp

#endif
