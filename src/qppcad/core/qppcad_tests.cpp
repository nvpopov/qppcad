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


  SECTION ("test get_root") {

    history_stream_t<int> hints1, hints2, hints3;
    hints3.set_parent(&hints2);
    hints2.set_parent(&hints1);
    REQUIRE(hints3.get_root() == &hints1);
    REQUIRE(hints2.get_root() == &hints1);
    REQUIRE(hints1.get_root() == &hints1);

  }

  SECTION ("test aggregator") {

    history_stream_t<int> hroot;

    history_stream_t<int> hints1;
    hints1.push_epoch_with_data(0);
    hints1.push_epoch_with_data(3);
    hints1.push_epoch_with_data(5);
    hints1.push_epoch_with_data(6, 6);
    REQUIRE(hints1.get_max_epoch() == 4);

    history_stream_t<int> hints2;
    hints2.push_epoch_with_data(0);
    hints2.push_epoch_with_data(1);
    hints2.push_epoch_with_data(2);
    hints2.push_epoch_with_data(3);
    REQUIRE(hints2.get_max_epoch() == 4);

    hints1.set_parent(&hroot);
    hints2.set_parent(&hroot);

    hints1.print_debug();
    hints2.print_debug();

    REQUIRE(hints1.get_max_epoch() == 6);
    REQUIRE(hints2.get_max_epoch() == 4);
    REQUIRE(hroot.get_max_epoch() == 6);

  }

}
