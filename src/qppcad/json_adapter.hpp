#ifndef QPPCAD_JSON_ADAPTER
#define QPPCAD_JSON_ADAPTER
#include <qppcad/qppcad.hpp>
#include <fifo_map.hpp>
#include <nlohmann/json.hpp>

namespace qpp {

  namespace cad {

    template<class K, class V, class dummy_compare, class A>
    using workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

    using json =
    nlohmann::basic_json<workaround_fifo_map,
    std::vector,
    std::string,
    bool,
    std::int64_t,
    std::uint64_t,
    float>;

    const auto JSON_QPPCAD_VERSION = "qppcad_file_version";
    const auto JSON_WS_NAME        = "ws_name";
    const auto JSON_BG_CLR         = "bg_clr";
    const auto JSON_OBJECTS        = "objects";
    const auto JSON_WS_ITEM_NAME   = "name";
    const auto JSON_WS_ITEM_TYPE   = "type";
    const auto JSON_IS_VISIBLE     = "is_visible";
    const auto JSON_POS            = "pos";
    const auto JSON_DIM            = "dim";
    const auto JSON_SHOW_IMG_ATOMS = "show_img_atoms";
    const auto JSON_SHOW_IMG_BONDS = "show_img_bonds";
    const auto JSON_SHOW_BONDS     = "show_bonds";
    const auto JSON_SHOW_ATOMS     = "show_atoms";
    const auto JSON_BT_SHOW_DSBL   = "bt_show_disabled_records";
    const auto JSON_ATOM_SCALE     = "atom_scale";
    const auto JSON_BOND_SCALE     = "bond_scale";
    const auto JSON_CELL_COLOR     = "cell_color";
    const auto JSON_BONDING_TABLE  = "bonding_table";
    const auto JSON_ATOMS          = "atoms";
    const auto JSON_CELL           = "cell";

  }

}
#endif
