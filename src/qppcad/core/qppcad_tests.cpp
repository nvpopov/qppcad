#define CATCH_CONFIG_MAIN
#include <qppcad/core/history_stream.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("history stream test") {

  SECTION ("basic features") {

    history_stream_t<int> hints;
    hints.advance_epoch(2);
    hints.advance_epoch(3);
    hints.advance_epoch(5);

    REQUIRE(hints.get_cur_epoch() == 3);

  }

  SECTION ("basic features + gen epoch") {

    history_stream_t<int> hints;
    hints.advance_epoch(2);
    hints.advance_epoch(3);
    hints.advance_epoch(5);
    hints.advance_epoch(6, 10);

    REQUIRE(hints.get_cur_epoch() == 10);

  }

  SECTION ("basic features + gen epoch") {

    history_stream_t<int> hints;
    hints.advance_epoch(2);
    hints.advance_epoch(3);
    hints.advance_epoch(5);
    hints.advance_epoch(6, 2);

    REQUIRE(hints.get_cur_epoch() == 4);

  }

}
