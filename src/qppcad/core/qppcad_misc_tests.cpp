#define CATCH_CONFIG_MAIN
#include <qppcad/ui/color_map.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("color map tests") {

  SECTION ("basic featuress") {

    REQUIRE(color_map_helper::get_by_threshold(color_map_rainbow, 0.5f) == color_map_rainbow[127]);
    REQUIRE(color_map_helper::get_by_threshold(color_map_rainbow, 0) == color_map_rainbow[0]);
    REQUIRE(color_map_helper::get_by_threshold(color_map_rainbow, -1) == color_map_rainbow[0]);
    REQUIRE(color_map_helper::get_by_threshold(color_map_rainbow, 1) == color_map_rainbow[255]);
    REQUIRE(color_map_helper::get_by_threshold(color_map_rainbow, 2) == color_map_rainbow[255]);

  }

}
