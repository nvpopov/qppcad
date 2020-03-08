#define CATCH_CONFIG_MAIN
#include <qppcad/core/history_stream.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("history stream test") {

  SECTION ("basic features") {

    history_stream_t<int> hints;
    hints.push_epoch_with_data(2);
    hints.push_epoch_with_data(3);
    hints.push_epoch_with_data(5);

    REQUIRE(hints.get_cur_epoch() == 3);
    REQUIRE(hints.epoch_exists_in_current_history(0));
    REQUIRE(hints.epoch_exists_in_current_history(1));
    REQUIRE(hints.epoch_exists_in_current_history(2));
    REQUIRE(hints.epoch_exists_in_current_history(3));
    REQUIRE(!hints.epoch_exists_in_current_history(4));
    hints.print_debug();

    hints.set_cur_epoch(2);
    hints.push_epoch_with_data(34);
    hints.print_debug();
    REQUIRE(hints.get_cur_epoch() == 4);
    REQUIRE(hints.get_hist_size() == 4);

  }

  SECTION ("basic features + gen epoch") {

    history_stream_t<int> hints;
    hints.push_epoch_with_data(2);
    hints.push_epoch_with_data(3);
    hints.push_epoch_with_data(5);
    hints.push_epoch_with_data(6, 10);

    REQUIRE(hints.get_cur_epoch() == 10);
    REQUIRE(!hints.epoch_exists_in_current_history(5));
    REQUIRE(hints.epoch_exists_in_current_history(10));

  }

  SECTION ("basic features + gen epoch") {

    history_stream_t<int> hints;
    hints.push_epoch_with_data(2);
    hints.push_epoch_with_data(3);
    hints.push_epoch_with_data(5);
    hints.push_epoch_with_data(6, 2);

    REQUIRE(hints.get_cur_epoch() == 4);

  }

  SECTION ("test aggregator") {

    history_stream_t<int> hints;
    hints.push_epoch_with_data(2);
    hints.push_epoch_with_data(3);
    hints.push_epoch_with_data(5);
    hints.push_epoch_with_data(6, 2);

    history_stream_t<double> hdbls;
    hdbls.push_epoch_with_data(2);
    hdbls.push_epoch_with_data(3);
    hdbls.push_epoch_with_data(5);
    hdbls.push_epoch_with_data(6, 2);


  }

}
