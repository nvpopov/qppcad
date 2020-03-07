#define CATCH_CONFIG_MAIN
#include <qppcad/core/history_stream.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("history stream test") {

  SECTION ("basic features") {

    history_stream_t<int> hints;
    hints.push_epoch(2);
    hints.push_epoch(3);
    hints.push_epoch(5);

    REQUIRE(hints.get_cur_epoch() == 3);
    REQUIRE(hints.epoch_exists_in_current_history(0));
    REQUIRE(hints.epoch_exists_in_current_history(1));
    REQUIRE(hints.epoch_exists_in_current_history(2));
    REQUIRE(hints.epoch_exists_in_current_history(3));
    REQUIRE(!hints.epoch_exists_in_current_history(4));
    hints.print_debug();

    hints.set_cur_epoch(2);
    hints.push_epoch(34);
    hints.print_debug();
    REQUIRE(hints.get_cur_epoch() == 4);
    REQUIRE(hints.get_cur_history_line_size() == 4);

  }

  SECTION ("basic features + gen epoch") {

    history_stream_t<int> hints;
    hints.push_epoch(2);
    hints.push_epoch(3);
    hints.push_epoch(5);
    hints.push_epoch(6, 10);

    REQUIRE(hints.get_cur_epoch() == 10);
    REQUIRE(!hints.epoch_exists_in_current_history(5));
    REQUIRE(hints.epoch_exists_in_current_history(10));

  }

  SECTION ("basic features + gen epoch") {

    history_stream_t<int> hints;
    hints.push_epoch(2);
    hints.push_epoch(3);
    hints.push_epoch(5);
    hints.push_epoch(6, 2);

    REQUIRE(hints.get_cur_epoch() == 4);

  }

}
