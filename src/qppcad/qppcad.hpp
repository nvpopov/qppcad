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
#include <qppcad/gitversion.h>

namespace qpp {

  namespace cad {

    typedef std::uint8_t  uint8_t;
    typedef std::uint16_t uint16_t;
    typedef std::uint32_t uint32_t;
    typedef std::uint64_t uint64_t;

    typedef std::int8_t  int8_t;
    typedef std::int16_t int16_t;
    typedef std::int32_t int32_t;
    typedef std::int64_t int64_t;

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
    const size_t max_recent_files = 10;

    /* error context flags*/
    const uint32_t error_ctx_def   = 0;
    const uint32_t error_ctx_throw = 1 << 1;
    const uint32_t error_ctx_mbox  = 1 << 2;

    enum cam_target_view_t : size_t {
      tv_x_axis,
      tv_y_axis,
      tv_z_axis,
      tv_cart_center,
      tv_a_axis,
      tv_b_axis,
      tv_c_axis,
      tv_a_star_axis,
      tv_b_star_axis,
      tv_c_star_axis,
      tv_auto
    };

  } // namespace qpp::cad

} // namespace qpp

#endif
