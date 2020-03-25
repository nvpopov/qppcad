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
    REQUIRE(hs->set_cur_epoch(2) == history_result_e::hr_invalid_epoch);

    REQUIRE(hs->push_epoch(1) == history_result_e::hr_success);
    REQUIRE(hs->push_epoch(2) == history_result_e::hr_success);

    REQUIRE(hs->get_history_size() == 3);

    history_document_base_t *hs_c1 = new history_document_base_t;
    history_document_base_t *hs_c2 = new history_document_base_t;
    history_document_base_t *hs_c3 = new history_document_base_t;
    history_document_base_t *hs_nc = new history_document_base_t;

    hs_c1->push_epoch(1);
    hs_c2->push_epoch(1);

    hs->add_child(hs_c1);
    hs->add_child(hs_c2);

    REQUIRE(hs->get_children_count() == 2);
    REQUIRE(*(hs->is_children(hs_c1)) == 0);
    REQUIRE(*(hs->is_children(hs_c2)) == 1);
    REQUIRE(hs->is_children(hs_nc) == std::nullopt);

    hs->add_child(hs_c3);
    REQUIRE(hs->get_children_count() == 3);
    hs->remove_child(hs_c3);
    REQUIRE(hs->get_children_count() == 2);

    hs->augment_epoch(hs_c1, 1, 1);
    REQUIRE(hs->get_augmented_count(1) == 1);

    hs->remove_augment_from_epoch(hs_c1, 1, 1);
    REQUIRE(hs->get_augmented_count(1) == 0);

  }

}
