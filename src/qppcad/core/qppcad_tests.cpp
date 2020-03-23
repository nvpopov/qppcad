#define CATCH_CONFIG_MAIN
#include <qppcad/core/history_stream.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("history stream test") {

  SECTION ("basic features") {

    history_document_base_t *hs = new history_document_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);
    REQUIRE(hs->set_cur_epoch(2) == false);

    REQUIRE(hs->push_epoch(1));
    REQUIRE(hs->push_epoch(2));

    REQUIRE(hs->get_history_size() == 3);

  }

}
