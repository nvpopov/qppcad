#ifndef QPP_QPPCAD
#define QPP_QPPCAD
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <chrono>
#include <ctime>
#include <set>
#include <optional>
#include <algorithm>
#include <functional>
#include <tuple>
#include <array>
#include <unordered_set>
#include <io/strfun.hpp>
#include <qppcad/core/gitversion.h>

namespace qpp {

  namespace cad {

    using size_t   = std::size_t;
    using uint8_t  = std::uint8_t;
    using uint16_t = std::uint16_t;
    using uint32_t = std::uint32_t;
    using uint64_t = std::uint64_t;

    using int8_t   = std::int8_t;
    using int16_t  = std::int16_t;
    using int32_t  = std::int32_t;
    using int64_t  = std::int64_t;

    /* alias for std::optional*/
    template<typename T>
    using opt = std::optional<T>;

    /**
     * @brief The spatial_units_e enum
     */
    enum spatial_units_e : int {
      spatial_units_ang,
      spatial_units_bohr
    };

    /**
     * @brief The ws_edit_e enum
     */
    enum ws_edit_e {
      edit_item = 0,
      edit_content = 1
    };

    /**
     * @brief max_recent_files
     */
    const size_t max_recent_files = 15;

    /* error context flags*/
    const uint32_t error_ctx_def    = 0;
    const uint32_t error_ctx_throw  = 1 << 1;
    const uint32_t error_ctx_mbox   = 1 << 2;
    const uint32_t error_ctx_ignore = 1 << 3;

    // camera target view
    enum cam_tv_e : size_t {
      tv_x,
      tv_y,
      tv_z,
      tv_cc,
      tv_a,
      tv_b,
      tv_c,
      tv_as,
      tv_bs,
      tv_cs,
      tv_auto
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
