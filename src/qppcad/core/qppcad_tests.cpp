#define CATCH_CONFIG_MAIN
#include <qppcad/core/history_stream.hpp>
#include <qppcad/core/history_stream_array_proxy.hpp>
#include <qppcad/core/history_stream_geom_adapter.hpp>
#include <qppcad/core/qppcad.hpp>
#include <catch.hpp>

using namespace qpp;
using namespace qpp::cad;

TEST_CASE("history stream test") {

  SECTION ("basic features") {

    hs_doc_base_t *hs = new hs_doc_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);
    REQUIRE(hs->set_cur_epoch(2) == hs_result_e::hs_invalid_epoch);
    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1});
    REQUIRE(hs->get_cur_epoch() == 0);
    REQUIRE(hs->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(2)) == hs_result_e::hs_success);

    REQUIRE(hs->get_history_size() == 3);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2});

    hs_doc_base_t *hs_c1 = new hs_doc_base_t;
    hs_doc_base_t *hs_c2 = new hs_doc_base_t;
    hs_doc_base_t *hs_c3 = new hs_doc_base_t;

    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs_c1->checkout_to_epoch(0) == hs_result_e::hs_success);

    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs_c2->checkout_to_epoch(0) == hs_result_e::hs_success);

    REQUIRE(hs->checkout_to_epoch(0) == hs_result_e::hs_success);
    hs->add_hs_child(hs_c1);
    hs->add_hs_child(hs_c2);

    REQUIRE(hs->checkout_to_epoch(0) == hs_result_e::hs_success);
    hs->add_hs_child(hs_c3);

  }

  SECTION ("basic features 2") {

    hs_doc_base_t *hs = new hs_doc_base_t;

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(2)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(3)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(3) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(4)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(4) == hs_result_e::hs_success);

    hs_doc_base_t *hs_c1 = new hs_doc_base_t;
    hs_doc_base_t *hs_c2 = new hs_doc_base_t;

    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs_c1->checkout_to_epoch(0) == hs_result_e::hs_success);

    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs_c2->checkout_to_epoch(0) == hs_result_e::hs_success);

    REQUIRE(hs->checkout_to_epoch(0) == hs_result_e::hs_success);

    REQUIRE(hs->add_hs_child(hs_c1) == hs_result_e::hs_success);
    REQUIRE(hs->add_hs_child(hs_c2) == hs_result_e::hs_success);
    REQUIRE(hs->add_hs_child(nullptr) == hs_result_e::hs_invalid_child);

  }

  SECTION("Deep nested structure") {

    hs_doc_base_t *hs       = new hs_doc_base_t;
    hs_doc_base_t *hs_c1    = new hs_doc_base_t;
    hs_doc_base_t *hs_c1_c1 = new hs_doc_base_t;
    hs_doc_base_t *hs_c1_c2 = new hs_doc_base_t;
    hs_doc_base_t *hs_c2    = new hs_doc_base_t;
    hs_doc_base_t *hs_c3    = new hs_doc_base_t;

    hs->add_hs_child(hs_c1);
    hs->add_hs_child(hs_c2);
    hs->add_hs_child(hs_c3);

    hs_c1->add_hs_child(hs_c1_c1);
    hs_c1->add_hs_child(hs_c1_c2);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_c1_c1->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_c1_c2->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_c3->push_epoch(1)) == hs_result_e::hs_success);

    //  current epoch augmented on add child
    //    hs->augment_epoch(hs_c1, 0, 0);
    //    hs->augment_epoch(hs_c2, 0, 0);
    //    hs->augment_epoch(hs_c3, 0, 0);
    hs->augment_epoch(1, hs_c1, 1);
    hs->augment_epoch(1, hs_c2, 1);
    hs->augment_epoch(1, hs_c3, 1);

    //hs_c1->augment_epoch(hs_c1_c1, 0, 0);
    //hs_c1->augment_epoch(hs_c1_c2, 0, 0);
    hs_c1->augment_epoch(1, hs_c1_c1, 1);
    hs_c1->augment_epoch(1, hs_c1_c2, 1);

    REQUIRE(hs->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hs_c1->get_cur_epoch() == 1);
    REQUIRE(hs_c2->get_cur_epoch() == 1);
    REQUIRE(hs_c3->get_cur_epoch() == 1);
    REQUIRE(hs_c1_c1->get_cur_epoch() == 1);
    REQUIRE(hs_c1_c2->get_cur_epoch() == 1);

    REQUIRE(hs->checkout_to_epoch(0) == hs_result_e::hs_success);
    REQUIRE(hs_c1->get_cur_epoch() == 0);
    REQUIRE(hs_c2->get_cur_epoch() == 0);
    REQUIRE(hs_c3->get_cur_epoch() == 0);
    REQUIRE(hs_c1_c1->get_cur_epoch() == 0);
    REQUIRE(hs_c1_c2->get_cur_epoch() == 0);

  }

  SECTION ("testing history cleanup") {

    hs_doc_base_t *hs = new hs_doc_base_t;

    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hs->get_cur_epoch() == 1);

    REQUIRE(std::get<0>(hs->push_epoch(2)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hs->get_cur_epoch() == 2);

    REQUIRE(std::get<0>(hs->push_epoch(3)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(3) == hs_result_e::hs_success);
    REQUIRE(hs->get_cur_epoch() == 3);

    hs_doc_base_t *hs1 = new hs_doc_base_t;

    REQUIRE(hs1->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs1->push_epoch()) == hs_result_e::hs_success);
    REQUIRE(hs1->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hs1->get_cur_epoch() == 1);

    REQUIRE(std::get<0>(hs1->push_epoch()) == hs_result_e::hs_success);
    REQUIRE(hs1->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hs1->get_cur_epoch() == 2);

    REQUIRE(std::get<0>(hs1->push_epoch()) == hs_result_e::hs_success);
    REQUIRE(hs1->checkout_to_epoch(3) == hs_result_e::hs_success);
    REQUIRE(hs1->get_cur_epoch() == 3);

  }

  SECTION ("hist doc test") {

    hs_doc_t<int> *hsi1 = new hs_doc_t<int>(0);
    hsi1->push_epoch_with_value(2);
    REQUIRE(hsi1->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hsi1->push_epoch_with_value(3) == hs_result_e::hs_success);
    REQUIRE(hsi1->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hsi1->push_epoch_with_value(4) == hs_result_e::hs_success);
    REQUIRE(hsi1->checkout_to_epoch(3) == hs_result_e::hs_success);

    REQUIRE(hsi1->get_history() == std::vector<epoch_t>{0, 1, 2, 3});

    REQUIRE(hsi1->get_history_size() == 4);
    REQUIRE(hsi1->get_cur_epoch() == 3);

    REQUIRE(hsi1->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hsi1->get_cur_epoch() == 1);
    REQUIRE(hsi1->get_value() == 2);

    REQUIRE(hsi1->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hsi1->get_cur_epoch() == 2);
    REQUIRE(hsi1->get_value() == 3);

    REQUIRE(hsi1->checkout_to_epoch(3) == hs_result_e::hs_success);
    REQUIRE(hsi1->get_cur_epoch() == 3);
    REQUIRE(hsi1->get_value() == 4);

    hs_doc_t<vector3<float>> *hsv1 = new hs_doc_t<vector3<float>>(vector3<float>(0));
    hsv1->push_epoch_with_value({0, 1, 0});
    REQUIRE(hsv1->checkout_to_epoch(1) == hs_result_e::hs_success);
    hsv1->push_epoch_with_value({0, 2, 0});

    REQUIRE(hsv1->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hsv1->get_cur_epoch() == 1);
    REQUIRE(hsv1->get_value() == vector3<float>{0, 1, 0});

    REQUIRE(hsv1->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hsv1->get_cur_epoch() == 2);
    REQUIRE(hsv1->get_value() ==  vector3<float>{0, 2, 0});

  }

  SECTION ("testing epoch cursor") {

    hs_doc_base_t *hs = new hs_doc_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(1) == hs_result_e::hs_success);

    REQUIRE(std::get<0>(hs->push_epoch(2)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(2) == hs_result_e::hs_success);

    REQUIRE(std::get<0>(hs->push_epoch(5)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(5) == hs_result_e::hs_success);

    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 5});
    REQUIRE(hs->get_history_size() == 4); //counts 0, 1, 2, 5

    REQUIRE(std::get<0>(hs->push_epoch(3)) == hs_result_e::hs_invalid_epoch);
    REQUIRE(hs->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(6)) == hs_result_e::hs_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 6});

  }

  SECTION ("nested elem deletions") {

    hs_doc_base_t *hs = new hs_doc_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(1) == hs_result_e::hs_success);

    REQUIRE(std::get<0>(hs->push_epoch(2)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(2) == hs_result_e::hs_success);

    REQUIRE(std::get<0>(hs->push_epoch(5)) == hs_result_e::hs_success);
    REQUIRE(hs->checkout_to_epoch(5) == hs_result_e::hs_success);

    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 5});
    REQUIRE(hs->get_history_size() == 4); //counts 0, 1, 2, 5

    REQUIRE(std::get<0>(hs->push_epoch(3)) == hs_result_e::hs_invalid_epoch);
    REQUIRE(hs->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(6)) == hs_result_e::hs_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 6});

    hs_doc_base_t *hs_c1 = new hs_doc_base_t;

    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(hs_c1->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hs_c1->get_cur_epoch() == 1);

    REQUIRE(std::get<0>(hs_c1->push_epoch(2)) == hs_result_e::hs_success);
    REQUIRE(hs_c1->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hs_c1->get_cur_epoch() == 2);

    hs_doc_base_t *hs_c2 = new hs_doc_base_t;
    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_c2->push_epoch(2)) == hs_result_e::hs_success);

    hs->add_hs_child(hs_c1);
    hs->add_hs_child(hs_c2);

    hs->augment_epoch(2, hs_c1, 1);
    hs->augment_epoch(2, hs_c2, 1);

    REQUIRE(hs->get_augmented_count(0) == 2);
    REQUIRE(hs->get_augmented_count(1) == 2);
    REQUIRE(hs->get_augmented_count(2) == 2);
    REQUIRE(hs->get_augmented_count(5) == 0);

    //std::cout << "@@@ SEC1" << std::endl;

    //    REQUIRE(hs->remove_child(hs_c1) == hs_result_e::hs_success);
    //    REQUIRE(hs->remove_child(hs_c2) == hs_result_e::hs_success);

    //    REQUIRE(hs->get_augmented_count(0) == 0);
    //    REQUIRE(hs->get_augmented_count(1) == 0);
    //    REQUIRE(hs->get_augmented_count(2) == 0);
    //    REQUIRE(hs->get_augmented_count(5) == 0);

  }

  SECTION ("nested elem deletions") {

    hs_doc_base_t *hs = new hs_doc_base_t;
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 3, 4});

  }

  SECTION ("nested elem deletions") {

    hs_doc_t<int> *hsi1 = new hs_doc_t<int>(0);

    hsi1->set_value(10);
    REQUIRE(hsi1->commit_exclusive() == hs_result_e::hs_success);

    hsi1->set_value(20);
    REQUIRE(hsi1->commit_exclusive() == hs_result_e::hs_success);

    hsi1->checkout_to_epoch(1);
    REQUIRE(hsi1->get_value() == 10);

    hsi1->checkout_to_epoch(2);
    REQUIRE(hsi1->get_value() == 20);

    REQUIRE(hsi1->can_checkout_by_dist(1) == false);
    REQUIRE(hsi1->can_checkout_by_dist(-1) == true);

    hsi1->checkout_to_epoch(1);
    REQUIRE(hsi1->can_checkout_by_dist(1) == true);
    REQUIRE(hsi1->can_checkout_by_dist(-1)== true);

    hsi1->checkout_to_epoch(0);
    REQUIRE(hsi1->can_checkout_by_dist(1) == true);
    REQUIRE(hsi1->can_checkout_by_dist(-1) == false);

  }

  SECTION ("nested elem exclusive commit") {

    hs_doc_t<int> *hsi1 = new hs_doc_t<int>(0);

    hsi1->set_value(10);
    REQUIRE(hsi1->commit_exclusive() == hs_result_e::hs_success);

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hs_doc_base_t *hs_el1 = new hs_doc_base_t;
    hs_doc_base_t *hs_el2 = new hs_doc_base_t;

    REQUIRE(hs_root->add_hs_child(hs_el1) == hs_result_e::hs_success);
    REQUIRE(hs_el1->add_hs_child(hs_el2) == hs_result_e::hs_success);
    REQUIRE(hs_el2->add_hs_child(hsi1) == hs_result_e::hs_success);

    hsi1->set_value(20);
    REQUIRE(hsi1->commit_exclusive() == hs_result_e::hs_success);
    REQUIRE(hsi1->get_cur_epoch() == 2);
    REQUIRE(hs_root->get_cur_epoch() == 1);
    REQUIRE(hs_el1->get_cur_epoch() == 1);
    REQUIRE(hs_el2->get_cur_epoch() == 1);

    hsi1->set_value(30);
    REQUIRE(hsi1->commit_exclusive() == hs_result_e::hs_success);
    REQUIRE(hsi1->get_cur_epoch() == 3);
    REQUIRE(hs_root->get_cur_epoch() == 2);
    REQUIRE(hs_el1->get_cur_epoch() == 2);
    REQUIRE(hs_el2->get_cur_epoch() == 2);

    REQUIRE(hs_root->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(hs_el1->get_cur_epoch() == 1);
    REQUIRE(hs_el2->get_cur_epoch() == 1);
    REQUIRE(hsi1->get_cur_epoch() == 2);
    REQUIRE(hsi1->get_value() == 20);

    REQUIRE(hs_root->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(hs_el1->get_cur_epoch() == 2);
    REQUIRE(hs_el2->get_cur_epoch() == 2);
    REQUIRE(hsi1->get_cur_epoch() == 3);
    REQUIRE(hsi1->get_value() == 30);

    REQUIRE(hsi1->commit_value_exclusive(44) == hs_result_e::hs_success);
    REQUIRE(hs_root->get_cur_epoch() == 3);
    REQUIRE(hsi1->commit_value_exclusive(45) == hs_result_e::hs_success);
    REQUIRE(hs_root->get_cur_epoch() == 4);

  }

  SECTION ("nested elem addition vol2") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hs_doc_t<int> *hs_el1 = new hs_doc_t<int>(0);
    hs_doc_t<int> *hs_el2 = new hs_doc_t<int>(0);

    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);

    REQUIRE(hs_root->add_hs_child(hs_el1) == hs_result_e::hs_success);
    REQUIRE(hs_root->add_hs_child(hs_el2) == hs_result_e::hs_success);

    REQUIRE(hs_root->is_child_alive(0, hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(1, hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(2, hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(3, hs_el1) == hs_result_e::hs_alive);

    REQUIRE(hs_root->is_child_alive(0, hs_el2) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(1, hs_el2) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(2, hs_el2) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(3, hs_el1) == hs_result_e::hs_alive);

    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);

    REQUIRE(hs_root->is_child_alive(0, hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(1, hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(2, hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(3, hs_el1) == hs_result_e::hs_alive);
    REQUIRE(hs_root->is_child_alive(4, hs_el1) == hs_result_e::hs_alive);
    REQUIRE(hs_root->is_child_alive(5, hs_el1) == hs_result_e::hs_alive);

    REQUIRE(hs_root->is_child_alive(0, hs_el2) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(1, hs_el2) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(2, hs_el2) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(3, hs_el2) == hs_result_e::hs_alive);
    REQUIRE(hs_root->is_child_alive(4, hs_el2) == hs_result_e::hs_alive);
    REQUIRE(hs_root->is_child_alive(5, hs_el2) == hs_result_e::hs_alive);

    REQUIRE(hs_el1->is_unmodified() == true);
    REQUIRE(hs_el2->is_unmodified() == true);
    REQUIRE(hs_root->is_unmodified() == true);

    hs_el1->set_value(10);
    REQUIRE(hs_el1->is_unmodified() == false);
    REQUIRE(hs_el2->is_unmodified() == true);
    REQUIRE(hs_root->is_unmodified() == false);

    REQUIRE(hs_root->reset() == hs_result_e::hs_success);
    REQUIRE(hs_el1->is_unmodified() == true);
    REQUIRE(hs_el2->is_unmodified() == true);
    REQUIRE(hs_root->is_unmodified() == true);

  }

  SECTION ("record test") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hs_doc_base_t *hs_it1 = new hs_doc_base_t;
    hs_doc_t<int> *hs_el1 = new hs_doc_t<int>(0);
    hs_doc_t<int> *hs_el2 = new hs_doc_t<int>(0);

    hs_it1->begin_recording(true); // init as base epoch

    REQUIRE(hs_it1->add_hs_child(hs_el1) == hs_result_e::hs_success);
    REQUIRE(hs_it1->add_hs_child(hs_el2) == hs_result_e::hs_success);

    hs_el1->set_value(25);
    hs_el2->set_value(42);

    hs_it1->end_recording();

    REQUIRE(hs_el1->get_cur_epoch() == 0);
    REQUIRE(hs_el2->get_cur_epoch() == 0);
    REQUIRE(hs_el1->is_unmodified());
    REQUIRE(hs_el2->is_unmodified());
    REQUIRE(hs_el1->get_value() == 25);
    REQUIRE(hs_el2->get_value() == 42);
    REQUIRE(hs_root->add_hs_child(hs_it1) == hs_result_e::hs_success);

    hs_root->set_commit_exclusive_on_change(true);
    hs_el1->set_value(122);
    hs_el1->set_value(222);

    REQUIRE(hs_el1->get_cur_epoch() == 4);

    REQUIRE(hs_root->checkout_by_dist(-1) == hs_result_e::hs_success);
    hs_el1->set_value(322);

  }

  SECTION ("init as base commit") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hs_doc_base_t *hs_it1 = new hs_doc_base_t;
    hs_doc_t<int> *hs_el1 = new hs_doc_t<int>(0);
    hs_doc_t<int> *hs_el2 = new hs_doc_t<int>(0);

    hs_it1->begin_recording(true); // init as base epoch

    REQUIRE(hs_it1->add_hs_child(hs_el1) == hs_result_e::hs_success);
    REQUIRE(hs_it1->add_hs_child(hs_el2) == hs_result_e::hs_success);

    hs_el1->set_value(25);
    hs_el2->set_value(42);

    hs_it1->end_recording();

    REQUIRE(hs_root->add_hs_child(hs_it1) == hs_result_e::hs_success);
    hs_root->set_commit_exclusive_on_change(true);

    hs_el1->set_value(23);
    hs_el2->set_value(26);

    REQUIRE(hs_root->checkout_by_dist(-1) == hs_result_e::hs_success);
    REQUIRE(hs_root->checkout_by_dist(-1) == hs_result_e::hs_success);

    REQUIRE(hs_el1->get_value() == 25);
    REQUIRE(hs_el1->get_cur_epoch() == 0);

  }

  SECTION ("add hs new epoch") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hs_doc_base_t *hs_el1 = new hs_doc_base_t;
    hs_doc_base_t *hs_el2 = new hs_doc_base_t;

    hs_root->add_hs_child(hs_el1, true);
    REQUIRE(hs_root->get_cur_epoch() == 1);

    hs_root->add_hs_child(hs_el2, true);
    REQUIRE(hs_root->get_cur_epoch() == 2);

    REQUIRE(hs_el1->commit_exclusive() == hs_result_e::hs_success);
    REQUIRE(hs_el1->commit_exclusive() == hs_result_e::hs_success);
    REQUIRE(hs_el1->commit_exclusive() == hs_result_e::hs_success);
    REQUIRE(hs_root->get_hs_children_count() == 2);
    REQUIRE(hs_root->get_hs_child(0) == hs_el1);
    REQUIRE(hs_root->get_hs_child(1) == hs_el2);

    REQUIRE(hs_root->set_alive_hs_child(hs_el1, false) == hs_result_e::hs_success);
    REQUIRE(hs_root->get_hs_children_count() == 1);
    REQUIRE(hs_root->get_hs_child(0) == hs_el2);
    REQUIRE(hs_root->get_cur_epoch() == 6);
    REQUIRE(hs_root->is_child_alive(hs_root->get_cur_epoch(), hs_el1) == hs_result_e::hs_dead);

    REQUIRE(hs_root->checkout_by_dist(-1) == hs_result_e::hs_success);
    REQUIRE(hs_root->is_child_alive(hs_root->get_cur_epoch(), hs_el1) == hs_result_e::hs_alive);

  }

  SECTION ("add hs new epoch multi elem delete") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hs_doc_base_t *hs_el1 = new hs_doc_base_t;
    hs_doc_base_t *hs_el2 = new hs_doc_base_t;
    hs_doc_base_t *hs_el3 = new hs_doc_base_t;
    hs_doc_base_t *hs_el4 = new hs_doc_base_t;

    hs_root->add_hs_child(hs_el1, true);
    REQUIRE(hs_root->get_cur_epoch() == 1);

    hs_root->add_hs_child(hs_el2, true);
    REQUIRE(hs_root->get_cur_epoch() == 2);

    hs_root->add_hs_child(hs_el3, true);
    REQUIRE(hs_root->get_cur_epoch() == 3);

    hs_root->add_hs_child(hs_el4, true);
    REQUIRE(hs_root->get_cur_epoch() == 4);

    REQUIRE(hs_root->set_alive_hs_child(hs_el1, false) == hs_result_e::hs_success);
    REQUIRE(hs_root->set_alive_hs_child(hs_el3, false) == hs_result_e::hs_success);
    REQUIRE(hs_root->get_hs_children_count() == 2);
    REQUIRE(hs_root->is_child_alive(hs_root->get_cur_epoch(), hs_el1) == hs_result_e::hs_dead);
    REQUIRE(hs_root->is_child_alive(hs_root->get_cur_epoch(), hs_el3) == hs_result_e::hs_dead);
    REQUIRE(hs_root->get_hs_child(0) == hs_el2);
    REQUIRE(hs_root->get_hs_child(1) == hs_el4);

  }

  SECTION ("array_proxy test") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hist_doc_array_proxy_t<hs_prop_int_t> *hs_iarray = new hist_doc_array_proxy_t<hs_prop_int_t>;

    REQUIRE(hs_root->add_hs_child(hs_iarray) == hs_result_e::hs_success);

    hs_prop_int_t *hs_int0 = new hs_prop_int_t;
    hs_prop_int_t *hs_int1 = new hs_prop_int_t;
    hs_prop_int_t *hs_int2 = new hs_prop_int_t;
    hs_prop_int_t *hs_int3 = new hs_prop_int_t;

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int0) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int1) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int2) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int3) == hs_result_e::hs_success);

    REQUIRE(hs_iarray->get_hs_child_as_array(0) == hs_int0);
    REQUIRE(hs_iarray->get_hs_child_as_array(1) == hs_int1);
    REQUIRE(hs_iarray->get_hs_child_as_array(2) == hs_int2);
    REQUIRE(hs_iarray->get_hs_child_as_array(3) == hs_int3);

  }

  SECTION ("is child unused tests") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;
    hist_doc_array_proxy_t<hs_prop_int_t> *hs_iarray = new hist_doc_array_proxy_t<hs_prop_int_t>;
    hs_iarray->set_auto_delete_children(true);

    REQUIRE(hs_root->add_hs_child(hs_iarray) == hs_result_e::hs_success); // 1
    hs_root->set_commit_exclusive_on_change(true);

    hs_prop_int_t *hs_int0 = new hs_prop_int_t;
    hs_int0->set_auto_delete(true);

    hs_prop_int_t *hs_int1 = new hs_prop_int_t;
    hs_int1->set_auto_delete(true);

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int0) == hs_result_e::hs_success); // 2
    REQUIRE(hs_iarray->get_super_parent() == hs_root);
    REQUIRE(hs_iarray->is_child_unused(hs_int0) == false);

    REQUIRE(hs_int0->get_super_parent() == hs_root);
    //REQUIRE(std::get<0>(hs_iarray->push_epoch()) == hs_result_e::hs_success); // 3
    REQUIRE(hs_root->get_cur_epoch() == 1);
    hs_int0->set_value(3);
    REQUIRE(hs_root->get_cur_epoch() == 2);

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int1) == hs_result_e::hs_success); // 4
    REQUIRE(hs_root->get_history_size() == 4);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);
    REQUIRE(hs_root->get_cur_epoch() == 3);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_alive);
    //Push epoch and cleanup case -> current issue
    REQUIRE(hs_iarray->get_hs_children_count() == 2);
    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);
    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_alive);
    REQUIRE(hs_iarray->is_child_unused(hs_int0) == false);
    REQUIRE(hs_iarray->is_child_unused(hs_int1) == false);
    REQUIRE(std::get<0>(hs_iarray->push_epoch()) == hs_result_e::hs_success); // 3
    REQUIRE(hs_iarray->is_child_unused(hs_int0) == false);
    REQUIRE(hs_iarray->is_child_unused(hs_int1) == false);
    REQUIRE(hs_iarray->get_hs_children_count() == 2);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);
    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_alive);
    //end of push epoch issue

    REQUIRE(hs_iarray->set_alive_hs_child(hs_int1, false) == hs_result_e::hs_success);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_dead);

    REQUIRE(hs_iarray->get_cur_epoch() == 5);
    REQUIRE(hs_root->checkout_to_epoch(1) == hs_result_e::hs_success); // 3
    REQUIRE(hs_iarray->get_cur_epoch() == 1);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_dead);

    //REQUIRE(!hs_iarray->is_child_unused(hs_int1));
    hs_int0->set_value(4);
    //REQUIRE(hs_iarray->is_child_unused(hs_int1));
    REQUIRE(hs_iarray->get_hs_children_count() == 1);
    //REQUIRE(hs_int1 == nullptr);
    //hs_int1->set_value(55);
    //    hs_root->set_commit_exclusive_on_change(true);
    //    hs_int0->set_value(42);
    //    REQUIRE(hs_iarray->get_cur_epoch() == 5);

  }

  SECTION ("is child unused tests - shared ptr storage") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;

    auto *hs_iarray =
        new hist_doc_array_proxy_t<hs_prop_int_t, hs_arr_sptr_policy<hs_prop_int_t>>();
    hs_iarray->set_auto_delete_children(true);

    REQUIRE(hs_root->add_hs_child(hs_iarray) == hs_result_e::hs_success); // 1
    hs_root->set_commit_exclusive_on_change(true);

    auto hs_int0 = std::make_shared<hs_prop_int_t>();
    hs_int0->set_auto_delete(true);

    auto hs_int1 = std::make_shared<hs_prop_int_t>();
    hs_int1->set_auto_delete(true);

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int0) == hs_result_e::hs_success); // 2
    REQUIRE(hs_iarray->get_super_parent() == hs_root);
    REQUIRE(hs_iarray->is_child_unused(hs_int0) == false);

    REQUIRE(hs_int0->get_super_parent() == hs_root);
    //REQUIRE(std::get<0>(hs_iarray->push_epoch()) == hs_result_e::hs_success); // 3
    REQUIRE(hs_root->get_cur_epoch() == 1);
    hs_int0->set_value(3);
    REQUIRE(hs_root->get_cur_epoch() == 2);

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int1) == hs_result_e::hs_success); // 4
    REQUIRE(hs_root->get_history_size() == 4);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);
    REQUIRE(hs_root->get_cur_epoch() == 3);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_alive);
    //Push epoch and cleanup case -> current issue
    REQUIRE(hs_iarray->get_hs_children_count() == 2);
    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);
    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_alive);
    REQUIRE(hs_iarray->is_child_unused(hs_int0) == false);
    REQUIRE(hs_iarray->is_child_unused(hs_int1) == false);
    REQUIRE(std::get<0>(hs_iarray->push_epoch()) == hs_result_e::hs_success); // 3
    REQUIRE(hs_iarray->is_child_unused(hs_int0) == false);
    REQUIRE(hs_iarray->is_child_unused(hs_int1) == false);
    REQUIRE(hs_iarray->get_hs_children_count() == 2);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);
    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_alive);
    REQUIRE(hs_int1.use_count() == 3); // 1 from current area, 1 from vector, 1 from map
    //end of push epoch issue

    REQUIRE(hs_iarray->set_alive_hs_child(hs_int1, false) == hs_result_e::hs_success);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_dead);

    REQUIRE(hs_iarray->get_cur_epoch() == 5);
    REQUIRE(hs_root->checkout_to_epoch(1) == hs_result_e::hs_success); // 3
    REQUIRE(hs_iarray->get_cur_epoch() == 1);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int0)
            == hs_result_e::hs_alive);

    REQUIRE(hs_iarray->is_child_alive(hs_iarray->get_cur_epoch(), hs_int1)
            == hs_result_e::hs_dead);

    //REQUIRE(!hs_iarray->is_child_unused(hs_int1));
    hs_int0->set_value(4);
    //REQUIRE(hs_iarray->is_child_unused(hs_int1));
    REQUIRE(hs_iarray->get_hs_children_count() == 1);
    REQUIRE(hs_int1.use_count() == 1);

  }

  SECTION ("is child unused tests - shared ptr storage") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;

    auto *hs_iarray =
        new hist_doc_array_proxy_t<hs_prop_int_t, hs_arr_sptr_policy<hs_prop_int_t>>();
    //hs_iarray->set_auto_delete_children(true);

    REQUIRE(hs_root->add_hs_child(hs_iarray) == hs_result_e::hs_success); // 1
    //hs_root->set_commit_exclusive_on_change(false);

    auto hs_int0 = std::make_shared<hs_prop_int_t>();
    //hs_int0->set_auto_delete(true);

    auto hs_int1 = std::make_shared<hs_prop_int_t>();
    //hs_int1->set_auto_delete(true);

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int0) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int1) == hs_result_e::hs_success);

  }

  SECTION ("is child unused tests - shared ptr storage") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;

    auto *hs_iarray =
        new hist_doc_array_proxy_t<hs_prop_int_t, hs_arr_sptr_policy<hs_prop_int_t>>();
    //hs_iarray->set_auto_delete_children(true);

    REQUIRE(hs_root->add_hs_child(hs_iarray) == hs_result_e::hs_success); // 1
    //hs_root->set_commit_exclusive_on_change(false);

    auto hs_int0 = std::make_shared<hs_prop_int_t>();
    //hs_int0->set_auto_delete(true);

    auto hs_int1 = std::make_shared<hs_prop_int_t>();
    //hs_int1->set_auto_delete(true);

    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int0) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int1) == hs_result_e::hs_success);

    hs_int0->set_value(5);
    hs_int0->set_value(6);
    hs_int0->set_value(7);

    REQUIRE(hs_root->squash() == hs_result_e::hs_success);
    REQUIRE(hs_root->get_cur_epoch() == 0);
    REQUIRE(hs_root->get_history_size() == 1);

    REQUIRE(hs_iarray->get_cur_epoch() == 0);
    REQUIRE(hs_iarray->get_history_size() == 1);

    REQUIRE(hs_int0->get_cur_epoch() == 0);
    REQUIRE(hs_int0->get_history_size() == 1);

    REQUIRE(hs_int1->get_cur_epoch() == 0);
    REQUIRE(hs_int1->get_history_size() == 1);

    REQUIRE(hs_int0->get_value() == 7);

  }

  SECTION ("delta state tests") {

    hs_doc_base_t *hs1 = new hs_doc_base_t;
    hs1->set_dstate_type(hs_dstate_e::hs_dstate_incr);

    REQUIRE(hs1->get_dstate_type() == hs_dstate_e::hs_dstate_incr);
    REQUIRE(std::get<0>(hs1->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs1->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(std::get<0>(hs1->push_epoch(std::nullopt, true)) == hs_result_e::hs_success);
    REQUIRE(hs1->get_history() == std::vector<epoch_t>{0, 1, 2, 3});

    REQUIRE(hs1->get_cur_epoch() == 3);
    REQUIRE(hs1->checkout_to_epoch(0) == hs_result_e::hs_success);
    REQUIRE(hs1->get_cur_epoch() == 0);
    REQUIRE(hs1->checkout_to_epoch(3) == hs_result_e::hs_success);
    REQUIRE(hs1->get_cur_epoch() == 3);

  }

  SECTION ("testing temporary objects") {

    hs_doc_base_t *hs_root = new hs_doc_base_t;

    auto *hs_iarray =
        new hist_doc_array_proxy_t<hs_prop_int_t, hs_arr_sptr_policy<hs_prop_int_t>>();
    //hs_iarray->set_auto_delete_children(true);

    REQUIRE(hs_root->add_hs_child(hs_iarray, true) == hs_result_e::hs_success); // 1
    //hs_root->set_commit_exclusive_on_change(false);

    auto hs_int0 = std::make_shared<hs_prop_int_t>();
    //hs_int0->set_auto_delete(true);

    auto hs_int1 = std::make_shared<hs_prop_int_t>();
    hs_int1->set_doctype(hs_doc_type_e::hs_doc_temporary);
    //hs_int1->set_auto_delete(true);

    REQUIRE(hs_root->get_cur_epoch() == 1);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int0) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->get_hs_children_count() == 1);
    REQUIRE(hs_root->get_cur_epoch() == 2);
    REQUIRE(hs_iarray->add_hs_child_as_array(hs_int1) == hs_result_e::hs_success);
    REQUIRE(hs_iarray->get_hs_children_count() == 2);
    REQUIRE(hs_root->get_cur_epoch() == 2);
    REQUIRE(hs_iarray->hs_remove_child_force(hs_int1) == hs_result_e::hs_success);
    REQUIRE(hs_root->get_cur_epoch() == 2);
    REQUIRE(hs_int1.use_count() == 1);

  }

  SECTION ("xgeom proxy") {

    using hs_xg_t = hist_doc_xgeom_proxy_t<double, qpp::periodic_cell<double>>;
    hs_xg_t *hs_xg = new hs_xg_t;
    xgeometry<double, periodic_cell<double>> xg1(0);
    hs_xg->set_xgeom(&xg1);

    REQUIRE(hs_xg->get_dstate_type() == hs_dstate_e::hs_dstate_incr);

    hs_xg->begin_editing();
    xg1.add("C", vector3<double>{0});
    hs_xg->end_editing();

    REQUIRE(xg1.nat() == 1);
    REQUIRE(hs_xg->get_cur_epoch() == 1);

    hs_xg->begin_editing();
    xg1.add("S",  vector3<double>{1});
    xg1.add("Ca", vector3<double>{2});
    xg1.add("F",  vector3<double>{3});
    hs_xg->end_editing();

    REQUIRE(hs_xg->get_cur_epoch() == 2);
    REQUIRE(xg1.nat() == 4);

    REQUIRE(hs_xg->checkout_to_epoch(1) == hs_result_e::hs_success);
    REQUIRE(xg1.nat() == 1);
    REQUIRE(hs_xg->get_cur_epoch() == 1);

    REQUIRE(hs_xg->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(xg1.nat() == 4);
    REQUIRE(hs_xg->get_cur_epoch() == 2);
    REQUIRE(xg1.pos(0) == vector3<double>{0});
    REQUIRE(xg1.atom_of_type(xg1.type(0)) == "C");
    REQUIRE(xg1.pos(1) == vector3<double>{1});
    REQUIRE(xg1.atom_of_type(xg1.type(1)) == "S");
    REQUIRE(xg1.pos(2) == vector3<double>{2});
    REQUIRE(xg1.atom_of_type(xg1.type(2)) == "Ca");
    REQUIRE(xg1.pos(3) == vector3<double>{3});
    REQUIRE(xg1.atom_of_type(xg1.type(3)) == "F");

    //testing erase
    hs_xg->begin_editing();
    xg1.erase(0);
    hs_xg->end_editing();
    REQUIRE(xg1.nat() == 3);
    REQUIRE(hs_xg->get_cur_epoch() == 3);
    REQUIRE(xg1.atom_of_type(xg1.type(0)) == "S");
    REQUIRE(xg1.atom_of_type(xg1.type(1)) == "Ca");
    REQUIRE(xg1.atom_of_type(xg1.type(2)) == "F");

    REQUIRE(hs_xg->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(xg1.nat() == 4);
    REQUIRE(xg1.atom_of_type(xg1.type(0)) == "C");
    REQUIRE(xg1.atom_of_type(xg1.type(1)) == "S");
    REQUIRE(xg1.atom_of_type(xg1.type(2)) == "Ca");
    REQUIRE(xg1.atom_of_type(xg1.type(3)) == "F");
    REQUIRE(hs_xg->checkout_to_epoch(2) == hs_result_e::hs_success);

    hs_xg->begin_editing();
    xg1.erase(0);
    hs_xg->end_editing();
    REQUIRE(hs_xg->get_cur_epoch() == 4);
    REQUIRE(xg1.nat() == 3);
    REQUIRE(hs_xg->checkout_to_epoch(2) == hs_result_e::hs_success);
    REQUIRE(xg1.nat() == 4);

    hs_xg->begin_editing();
    xg1.insert(1, "He", vector3<double>{0, 10, 10});
    hs_xg->end_editing();

    REQUIRE(hs_xg->get_cur_epoch() == 5);
    REQUIRE(xg1.atom_of_type(xg1.type(0)) == "C");
    REQUIRE(xg1.atom_of_type(xg1.type(1)) == "He");
    REQUIRE(xg1.atom_of_type(xg1.type(2)) == "S");
    REQUIRE(xg1.atom_of_type(xg1.type(3)) == "Ca");
    REQUIRE(xg1.atom_of_type(xg1.type(4)) == "F");

  }

  SECTION ("xgeom proxy - change cell/dim") {

    using hs_xg_t = hist_doc_xgeom_proxy_t<double, qpp::periodic_cell<double>>;
    hs_xg_t *hs_xg = new hs_xg_t;
    xgeometry<double, periodic_cell<double>> xg1(3);
    xg1.cell.v[0] = vector3<double>(5, 0, 0);
    xg1.cell.v[1] = vector3<double>(0, 5, 0);
    xg1.cell.v[2] = vector3<double>(0, 0, 5);

    hs_xg->set_xgeom(&xg1);

    REQUIRE(hs_xg->get_dstate_type() == hs_dstate_e::hs_dstate_incr);
    REQUIRE(hs_xg->get_cur_epoch() == 0);
    hs_xg->hs_change_DIM(xgeom_proxy_hs_act_type_e::hs_act_emit_both, 0);
    REQUIRE(hs_xg->get_cur_epoch() == 1);
    REQUIRE(xg1.get_DIM() == 0);

    hs_xg->hs_change_DIM(xgeom_proxy_hs_act_type_e::hs_act_emit_both, 3);
    REQUIRE(hs_xg->get_cur_epoch() == 2);
    REQUIRE(xg1.get_DIM() == 3);

    REQUIRE(xg1.cell.v[0] == vector3<double>(5, 0, 0));
    REQUIRE(xg1.cell.v[1] == vector3<double>(0, 5, 0));
    REQUIRE(xg1.cell.v[2] == vector3<double>(0, 0, 5));

    hs_xg->checkout_to_epoch(1);
    REQUIRE(xg1.get_DIM() == 0);
    hs_xg->checkout_to_epoch(2);
    REQUIRE(xg1.get_DIM() == 3);

    hs_xg->hs_change_cell(xgeom_proxy_hs_act_type_e::hs_act_emit_both,
                          vector3<double>(15,  0,  0),
                          vector3<double>( 0, 15,  0),
                          vector3<double>( 0,  0, 15));
    REQUIRE(hs_xg->get_cur_epoch() == 3);
    REQUIRE(xg1.cell.v[0] == vector3<double>(15,  0,  0));
    REQUIRE(xg1.cell.v[1] == vector3<double>( 0, 15,  0));
    REQUIRE(xg1.cell.v[2] == vector3<double>( 0,  0, 15));

    hs_xg->checkout_to_epoch(2);
    REQUIRE(xg1.cell.v[0] == vector3<double>(5, 0, 0));
    REQUIRE(xg1.cell.v[1] == vector3<double>(0, 5, 0));
    REQUIRE(xg1.cell.v[2] == vector3<double>(0, 0, 5));

  }

  SECTION ("xgeom proxy - change atom") {

    using hs_xg_t = hist_doc_xgeom_proxy_t<double, qpp::periodic_cell<double>>;
    hs_xg_t *hs_xg = new hs_xg_t;
    xgeometry<double, periodic_cell<double>> xg1(0);
    hs_xg->set_xgeom(&xg1);

    REQUIRE(hs_xg->get_dstate_type() == hs_dstate_e::hs_dstate_incr);

    hs_xg->begin_editing();
    xg1.add("C", vector3<double>{2, 3, 4});
    hs_xg->end_editing();

    REQUIRE(xg1.nat() == 1);
    REQUIRE(hs_xg->get_cur_epoch() == 1);

    hs_xg->begin_editing();
    xg1.add("S", vector3<double>{1});
    xg1.add("Ca", vector3<double>{2});
    xg1.add("F", vector3<double>{3});
    hs_xg->end_editing();

    xg1.change_pos(0, vector3<double>{12});

    REQUIRE(hs_xg->get_cur_epoch() == 3);
    REQUIRE(xg1.pos(0) == vector3<double>{12});

    hs_xg->checkout_to_epoch(2);
    REQUIRE(hs_xg->get_cur_epoch() == 2);
    REQUIRE(xg1.pos(0) == vector3<double>{2, 3, 4});

  }

  SECTION ("xgeom proxy - reorder tests") {

    using hs_xg_t = hist_doc_xgeom_proxy_t<double, qpp::periodic_cell<double>>;
    hs_xg_t *hs_xg = new hs_xg_t;
    xgeometry<double, periodic_cell<double>> xg1(0);
    hs_xg->set_xgeom(&xg1);

    REQUIRE(hs_xg->get_dstate_type() == hs_dstate_e::hs_dstate_incr);

    hs_xg->begin_editing();
    xg1.add("S0", vector3<double>{0});
    xg1.add("S1", vector3<double>{1});
    xg1.add("S2", vector3<double>{2});
    xg1.add("S3", vector3<double>{3});
    xg1.add("S4", vector3<double>{4});
    hs_xg->end_editing();

    REQUIRE(hs_xg->get_cur_epoch() == 1);
    REQUIRE(xg1.nat() == 5);

    xg1.reorder({4, 3, 2, 1, 0});
    REQUIRE(hs_xg->get_cur_epoch() == 2);
    REQUIRE(xg1.atom_of_type(xg1.type(0)) == "S4");
    REQUIRE(xg1.atom_of_type(xg1.type(1)) == "S3");
    REQUIRE(xg1.atom_of_type(xg1.type(2)) == "S2");
    REQUIRE(xg1.atom_of_type(xg1.type(3)) == "S1");
    REQUIRE(xg1.atom_of_type(xg1.type(4)) == "S0");

    hs_xg->checkout_to_epoch(1);
    REQUIRE(xg1.nat() == 5);
    REQUIRE(hs_xg->get_cur_epoch() == 1);
    REQUIRE(xg1.atom_of_type(xg1.type(0)) == "S0");
    REQUIRE(xg1.atom_of_type(xg1.type(1)) == "S1");
    REQUIRE(xg1.atom_of_type(xg1.type(2)) == "S2");
    REQUIRE(xg1.atom_of_type(xg1.type(3)) == "S3");
    REQUIRE(xg1.atom_of_type(xg1.type(4)) == "S4");

  }

  SECTION ("xgeom proxy - xfield tests") {

    using hs_xg_t = hist_doc_xgeom_proxy_t<double, qpp::periodic_cell<double>>;
    hs_xg_t *hs_xg = new hs_xg_t;
    xgeometry<double, periodic_cell<double>> xg1(0);

    xg1.set_format( {
                       "atom", //0
                       "x",//1
                       "y",//2
                       "z",//3
                       "charge",//4
                       "number", //5
                       "hide",//6
                       "lshow",//7
                       "r",//8
                       "g",//9
                       "b",//10
                       "ltext",//11
                       "ov",//12
                       "atom_r", // 13
                       "tag" //14
                   },

                   {
                       basic_types::type_string, //0
                       basic_types::type_real, //1
                       basic_types::type_real, //2
                       basic_types::type_real, //3
                       basic_types::type_real, //4
                       basic_types::type_int, //5
                       basic_types::type_bool, //6
                       basic_types::type_bool, //7
                       basic_types::type_real, //8
                       basic_types::type_real, //9
                       basic_types::type_real, //10
                       basic_types::type_string, //11
                       basic_types::type_bool, // 12
                       basic_types::type_real, // 13
                       basic_types::type_string //14
                   } );

    hs_xg->set_xgeom(&xg1);

    REQUIRE(hs_xg->get_dstate_type() == hs_dstate_e::hs_dstate_incr);

    hs_xg->begin_editing();
    xg1.add("S0", vector3<double>{0});
    xg1.add("S1", vector3<double>{1});
    xg1.add("S2", vector3<double>{2});
    xg1.add("S3", vector3<double>{3});
    xg1.add("S4", vector3<double>{4});
    hs_xg->end_editing();

    REQUIRE(hs_xg->get_cur_epoch() == 1);
    REQUIRE(xg1.nat() == 5);

    /* testing double */
    hs_xg->begin_editing();
    xg1.set_xfield<double>(4, 0, -20);
    hs_xg->end_editing();

    REQUIRE(hs_xg->get_cur_epoch() == 2);
    REQUIRE(xg1.get_xfield<double>(4, 0) == -20);

    xg1.set_xfield<double>(4, 0, -10);
    REQUIRE(hs_xg->get_cur_epoch() == 3);
    REQUIRE(xg1.get_xfield<double>(4, 0) == -10);

    hs_xg->checkout_to_epoch(2);
    REQUIRE(xg1.get_xfield<double>(4, 0) == -20);

    hs_xg->checkout_to_epoch(1);
    REQUIRE(xg1.get_xfield<double>(4, 0) == 0);

    /* testing string */
    REQUIRE(xg1.xfield<std::string>(0, 0) == "S0");
    xg1.set_xfield<std::string>(0, 0, "X0");
    REQUIRE(hs_xg->get_cur_epoch() == 4);
    REQUIRE(xg1.xfield<std::string>(0, 0) == "X0");
    hs_xg->checkout_to_epoch(1);
    REQUIRE(xg1.xfield<std::string>(0, 0) == "S0");

    /* testing int */
    REQUIRE(xg1.xfield<int>(5, 0) == 0);
    xg1.set_xfield<int>(5, 0, 1);
    REQUIRE(hs_xg->get_cur_epoch() == 5);
    REQUIRE(xg1.xfield<int>(5, 0) == 1);
    hs_xg->checkout_to_epoch(1);
    REQUIRE(xg1.xfield<int>(5, 0) == 0);

    /* testing bool */
    REQUIRE(xg1.xfield<bool>(6, 0) == false);
    xg1.set_xfield<bool>(6, 0, true);
    REQUIRE(hs_xg->get_cur_epoch() == 6);
    REQUIRE(xg1.xfield<bool>(6, 0) == true);
    hs_xg->checkout_to_epoch(1);
    REQUIRE(xg1.xfield<bool>(6, 0) == false);

  }

}
