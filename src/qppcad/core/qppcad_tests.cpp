#define CATCH_CONFIG_MAIN
#include <qppcad/core/history_stream.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("history stream test") {

  SECTION ("basic features") {

    hist_doc_base_t *hs = new hist_doc_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);
    REQUIRE(hs->set_cur_epoch(2) == hr_result_e::hr_invalid_epoch);

    REQUIRE(hs->push_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs->push_epoch(2) == hr_result_e::hr_success);

    REQUIRE(hs->get_history_size() == 3);

    hist_doc_base_t *hs_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c2 = new hist_doc_base_t;
    hist_doc_base_t *hs_c3 = new hist_doc_base_t;
    hist_doc_base_t *hs_nc = new hist_doc_base_t;

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

    //manual epoch constructing
    hs->augment_epoch(hs_c1, 0, 0);
    hs->augment_epoch(hs_c2, 0, 0);
    hs->augment_epoch(hs_c1, 1, 1);
    hs->augment_epoch(hs_c2, 1, 1);

    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 1);
    REQUIRE(hs_c2->get_cur_epoch() == 1);
    REQUIRE(hs->checkout_to_epoch(0) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 0);
    REQUIRE(hs_c2->get_cur_epoch() == 0);

    REQUIRE(hs_c1->get_root() == hs);
    REQUIRE(hs->get_root() == hs);

  }

  SECTION("Deep nested structure") {

    hist_doc_base_t *hs = new hist_doc_base_t;
    hist_doc_base_t *hs_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c1_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c1_c2 = new hist_doc_base_t;
    hist_doc_base_t *hs_c2 = new hist_doc_base_t;
    hist_doc_base_t *hs_c3 = new hist_doc_base_t;

    hs->add_child(hs_c1);
    hs->add_child(hs_c2);
    hs->add_child(hs_c3);

    hs_c1->add_child(hs_c1_c1);
    hs_c1->add_child(hs_c1_c2);

    hs->push_epoch(1);
    hs_c1->push_epoch(1);
    hs_c1_c1->push_epoch(1);
    hs_c1_c2->push_epoch(1);
    hs_c2->push_epoch(1);
    hs_c3->push_epoch(1);

    hs->augment_epoch(hs_c1, 0, 0);
    hs->augment_epoch(hs_c2, 0, 0);
    hs->augment_epoch(hs_c3, 0, 0);
    hs->augment_epoch(hs_c1, 1, 1);
    hs->augment_epoch(hs_c2, 1, 1);
    hs->augment_epoch(hs_c3, 1, 1);

    hs_c1->augment_epoch(hs_c1_c1, 0, 0);
    hs_c1->augment_epoch(hs_c1_c2, 0, 0);
    hs_c1->augment_epoch(hs_c1_c1, 1, 1);
    hs_c1->augment_epoch(hs_c1_c2, 1, 1);

    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 1);
    REQUIRE(hs_c2->get_cur_epoch() == 1);
    REQUIRE(hs_c3->get_cur_epoch() == 1);
    REQUIRE(hs_c1_c1->get_cur_epoch() == 1);
    REQUIRE(hs_c1_c2->get_cur_epoch() == 1);

    REQUIRE(hs->checkout_to_epoch(0) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 0);
    REQUIRE(hs_c2->get_cur_epoch() == 0);
    REQUIRE(hs_c3->get_cur_epoch() == 0);
    REQUIRE(hs_c1_c1->get_cur_epoch() == 0);
    REQUIRE(hs_c1_c2->get_cur_epoch() == 0);

  }

}
