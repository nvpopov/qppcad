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
    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(2)) == hr_result_e::hr_success);

    REQUIRE(hs->get_history_size() == 3);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2});

    hist_doc_base_t *hs_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c2 = new hist_doc_base_t;
    hist_doc_base_t *hs_c3 = new hist_doc_base_t;
    hist_doc_base_t *hs_nc = new hist_doc_base_t;

    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs_c1->checkout_to_epoch(0) == hr_result_e::hr_success);

    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs_c2->checkout_to_epoch(0) == hr_result_e::hr_success);

    REQUIRE(hs->checkout_to_epoch(0) == hr_result_e::hr_success);
    hs->add_hs_child(hs_c1);
    hs->add_hs_child(hs_c2);

    REQUIRE(hs->get_children_count() == 2);
    REQUIRE(*(hs->is_child(hs_c1)) == 0);
    REQUIRE(*(hs->is_child(hs_c2)) == 1);
    REQUIRE(hs->is_child(hs_nc) == std::nullopt);

    REQUIRE(hs->checkout_to_epoch(0) == hr_result_e::hr_success);
    hs->add_hs_child(hs_c3);
    REQUIRE(hs->get_children_count() == 3);
    hs->remove_child(hs_c3);
    REQUIRE(hs->get_children_count() == 2);

    hs->augment_epoch(1, hs_c1, 1);
    REQUIRE(hs->get_augmented_count(1) == 1); // added augments when we add as child

    hs->remove_augment_from_epoch(hs_c1, 1);
    REQUIRE(hs->get_augmented_count(1) == 0);

    REQUIRE(hs->augment_epoch(1, hs_c1, 2) == hr_result_e::hr_invalid_child_epoch);

    //manual epoch constructing
    //hs->augment_epoch(hs_c1, 0, 0); already
    //hs->augment_epoch(hs_c2, 0, 0); already
    hs->augment_epoch(1, hs_c1, 1);
    hs->augment_epoch(1, hs_c2, 1);

    REQUIRE(hs->get_augmented_count(1) == 2);
    REQUIRE(hs->get_augmented_count(0) == 2);

    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);

    //REQUIRE(hs_c1->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 1);

    //REQUIRE(hs_c2->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs_c2->get_cur_epoch() == 1);

    REQUIRE(hs->checkout_to_epoch(0) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 0);
    REQUIRE(hs_c2->get_cur_epoch() == 0);

    REQUIRE(hs_c1->get_parent() == hs);
    REQUIRE(hs->get_parent() == hs);

  }

  SECTION ("basic features 2") {

    hist_doc_base_t *hs = new hist_doc_base_t;

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(2)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(3)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(3) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(4)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(4) == hr_result_e::hr_success);

    hist_doc_base_t *hs_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c2 = new hist_doc_base_t;

    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs_c1->checkout_to_epoch(0) == hr_result_e::hr_success);

    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs_c2->checkout_to_epoch(0) == hr_result_e::hr_success);

    REQUIRE(hs->checkout_to_epoch(0) == hr_result_e::hr_success);

    REQUIRE(hs->add_hs_child(hs_c1) == hr_result_e::hr_success);
    REQUIRE(hs->add_hs_child(hs_c2) == hr_result_e::hr_success);
    REQUIRE(hs->add_hs_child(nullptr) == hr_result_e::hr_invalid_child);

  }

  SECTION("Deep nested structure") {

    hist_doc_base_t *hs = new hist_doc_base_t;
    hist_doc_base_t *hs_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c1_c1 = new hist_doc_base_t;
    hist_doc_base_t *hs_c1_c2 = new hist_doc_base_t;
    hist_doc_base_t *hs_c2 = new hist_doc_base_t;
    hist_doc_base_t *hs_c3 = new hist_doc_base_t;

    hs->add_hs_child(hs_c1);
    hs->add_hs_child(hs_c2);
    hs->add_hs_child(hs_c3);

    hs_c1->add_hs_child(hs_c1_c1);
    hs_c1->add_hs_child(hs_c1_c2);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_c1_c1->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_c1_c2->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_c3->push_epoch(1)) == hr_result_e::hr_success);

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

  SECTION ("testing history cleanup") {

    hist_doc_base_t *hs = new hist_doc_base_t;

    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs->get_cur_epoch() == 1);

    REQUIRE(std::get<0>(hs->push_epoch(2)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(hs->get_cur_epoch() == 2);

    REQUIRE(std::get<0>(hs->push_epoch(3)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(3) == hr_result_e::hr_success);
    REQUIRE(hs->get_cur_epoch() == 3);

    hist_doc_base_t *hs1 = new hist_doc_base_t;

    REQUIRE(hs1->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs1->push_epoch()) == hr_result_e::hr_success);
    REQUIRE(hs1->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs1->get_cur_epoch() == 1);

    REQUIRE(std::get<0>(hs1->push_epoch()) == hr_result_e::hr_success);
    REQUIRE(hs1->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(hs1->get_cur_epoch() == 2);

    REQUIRE(std::get<0>(hs1->push_epoch()) == hr_result_e::hr_success);
    REQUIRE(hs1->checkout_to_epoch(3) == hr_result_e::hr_success);
    REQUIRE(hs1->get_cur_epoch() == 3);

  }

  SECTION ("hist doc test") {

    hist_doc_t<int> *hsi1 = new hist_doc_t<int>(0);
    hsi1->push_epoch_with_value(2);
    REQUIRE(hsi1->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hsi1->push_epoch_with_value(3) == hr_result_e::hr_success);
    REQUIRE(hsi1->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(hsi1->push_epoch_with_value(4) == hr_result_e::hr_success);
    REQUIRE(hsi1->checkout_to_epoch(3) == hr_result_e::hr_success);

    REQUIRE(hsi1->get_history() == std::vector<epoch_t>{0, 1, 2, 3});

    REQUIRE(hsi1->get_history_size() == 4);
    REQUIRE(hsi1->get_cur_epoch() == 3);

    REQUIRE(hsi1->checkout_to_epoch(1));
    REQUIRE(hsi1->get_cur_epoch() == 1);
    REQUIRE(hsi1->get_value() == 2);

    REQUIRE(hsi1->checkout_to_epoch(2));
    REQUIRE(hsi1->get_cur_epoch() == 2);
    REQUIRE(hsi1->get_value() == 3);

    REQUIRE(hsi1->checkout_to_epoch(3));
    REQUIRE(hsi1->get_cur_epoch() == 3);
    REQUIRE(hsi1->get_value() == 4);

    hist_doc_t<vector3<float>> *hsv1 = new hist_doc_t<vector3<float>>(vector3<float>(0));
    hsv1->push_epoch_with_value({0, 1, 0});
    REQUIRE(hsv1->checkout_to_epoch(1) == hr_result_e::hr_success);
    hsv1->push_epoch_with_value({0, 2, 0});

    REQUIRE(hsv1->checkout_to_epoch(1));
    REQUIRE(hsv1->get_cur_epoch() == 1);
    REQUIRE(hsv1->get_value() == vector3<float>{0, 1, 0});

    REQUIRE(hsv1->checkout_to_epoch(2));
    REQUIRE(hsv1->get_cur_epoch() == 2);
    REQUIRE(hsv1->get_value() ==  vector3<float>{0, 2, 0});

  }

  SECTION ("testing epoch cursor") {

    hist_doc_base_t *hs = new hist_doc_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);

    REQUIRE(std::get<0>(hs->push_epoch(2)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(2) == hr_result_e::hr_success);

    REQUIRE(std::get<0>(hs->push_epoch(5)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(5) == hr_result_e::hr_success);

    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 5});
    REQUIRE(hs->get_history_size() == 4); //counts 0, 1, 2, 5

    REQUIRE(std::get<0>(hs->push_epoch(3)) == hr_result_e::hr_invalid_epoch);
    REQUIRE(hs->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(6)) == hr_result_e::hr_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 6});

  }

  SECTION ("nested elem deletions") {

    hist_doc_base_t *hs = new hist_doc_base_t;
    REQUIRE(hs->get_cur_epoch() == 0);

    REQUIRE(std::get<0>(hs->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(1) == hr_result_e::hr_success);

    REQUIRE(std::get<0>(hs->push_epoch(2)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(2) == hr_result_e::hr_success);

    REQUIRE(std::get<0>(hs->push_epoch(5)) == hr_result_e::hr_success);
    REQUIRE(hs->checkout_to_epoch(5) == hr_result_e::hr_success);

    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 5});
    REQUIRE(hs->get_history_size() == 4); //counts 0, 1, 2, 5

    REQUIRE(std::get<0>(hs->push_epoch(3)) == hr_result_e::hr_invalid_epoch);
    REQUIRE(hs->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(6)) == hr_result_e::hr_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 6});

    hist_doc_base_t *hs_c1 = new hist_doc_base_t;

    REQUIRE(std::get<0>(hs_c1->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(hs_c1->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 1);

    REQUIRE(std::get<0>(hs_c1->push_epoch(2)) == hr_result_e::hr_success);
    REQUIRE(hs_c1->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(hs_c1->get_cur_epoch() == 2);

    hist_doc_base_t *hs_c2 = new hist_doc_base_t;
    REQUIRE(std::get<0>(hs_c2->push_epoch(1)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_c2->push_epoch(2)) == hr_result_e::hr_success);

    hs->add_hs_child(hs_c1);
    hs->add_hs_child(hs_c2);

    hs->augment_epoch(2, hs_c1, 1);
    hs->augment_epoch(2, hs_c2, 1);

    REQUIRE(hs->get_augmented_count(0) == 2);
    REQUIRE(hs->get_augmented_count(1) == 2);
    REQUIRE(hs->get_augmented_count(2) == 2);
    REQUIRE(hs->get_augmented_count(5) == 0);

    //std::cout << "@@@ SEC1" << std::endl;

    REQUIRE(hs->remove_child(hs_c1) == hr_result_e::hr_success);
    REQUIRE(hs->remove_child(hs_c2) == hr_result_e::hr_success);

    REQUIRE(hs->get_augmented_count(0) == 0);
    REQUIRE(hs->get_augmented_count(1) == 0);
    REQUIRE(hs->get_augmented_count(2) == 0);
    REQUIRE(hs->get_augmented_count(5) == 0);

  }

  SECTION ("nested elem deletions") {

    hist_doc_base_t *hs = new hist_doc_base_t;
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(hs->get_history() == std::vector<epoch_t>{0, 1, 2, 3, 4});

  }

  SECTION ("nested elem deletions") {

    hist_doc_t<int> *hsi1 = new hist_doc_t<int>(0);

    hsi1->set_value(10);
    REQUIRE(hsi1->commit_exclusive() == hr_result_e::hr_success);

    hsi1->set_value(20);
    REQUIRE(hsi1->commit_exclusive() == hr_result_e::hr_success);

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

    hist_doc_t<int> *hsi1 = new hist_doc_t<int>(0);

    hsi1->set_value(10);
    REQUIRE(hsi1->commit_exclusive() == hr_result_e::hr_success);

    hist_doc_base_t *hs_root = new hist_doc_base_t;
    hist_doc_base_t *hs_el1 = new hist_doc_base_t;
    hist_doc_base_t *hs_el2 = new hist_doc_base_t;

    REQUIRE(hs_root->add_hs_child(hs_el1) == hr_result_e::hr_success);
    REQUIRE(hs_el1->add_hs_child(hs_el2) == hr_result_e::hr_success);
    REQUIRE(hs_el2->add_hs_child(hsi1) == hr_result_e::hr_success);

    hsi1->set_value(20);
    REQUIRE(hsi1->commit_exclusive() == hr_result_e::hr_success);
    REQUIRE(hsi1->get_cur_epoch() == 2);
    REQUIRE(hs_root->get_cur_epoch() == 1);
    REQUIRE(hs_el1->get_cur_epoch() == 1);
    REQUIRE(hs_el2->get_cur_epoch() == 1);

    hsi1->set_value(30);
    REQUIRE(hsi1->commit_exclusive() == hr_result_e::hr_success);
    REQUIRE(hsi1->get_cur_epoch() == 3);
    REQUIRE(hs_root->get_cur_epoch() == 2);
    REQUIRE(hs_el1->get_cur_epoch() == 2);
    REQUIRE(hs_el2->get_cur_epoch() == 2);

    REQUIRE(hs_root->checkout_to_epoch(1) == hr_result_e::hr_success);
    REQUIRE(hs_el1->get_cur_epoch() == 1);
    REQUIRE(hs_el2->get_cur_epoch() == 1);
    REQUIRE(hsi1->get_cur_epoch() == 2);
    REQUIRE(hsi1->get_value() == 20);

    REQUIRE(hs_root->checkout_to_epoch(2) == hr_result_e::hr_success);
    REQUIRE(hs_el1->get_cur_epoch() == 2);
    REQUIRE(hs_el2->get_cur_epoch() == 2);
    REQUIRE(hsi1->get_cur_epoch() == 3);
    REQUIRE(hsi1->get_value() == 30);

    REQUIRE(hsi1->commit_value_exclusive(44) == hr_result_e::hr_success);
    REQUIRE(hs_root->get_cur_epoch() == 3);
    REQUIRE(hsi1->commit_value_exclusive(45) == hr_result_e::hr_success);
    REQUIRE(hs_root->get_cur_epoch() == 4);

  }

  SECTION ("nested elem addition vol2") {

    hist_doc_base_t *hs_root = new hist_doc_base_t;
    hist_doc_t<int> *hs_el1 = new hist_doc_t<int>(0);
    hist_doc_t<int> *hs_el2 = new hist_doc_t<int>(0);

    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);

    REQUIRE(hs_root->add_hs_child(hs_el1) == hr_result_e::hr_success);
    REQUIRE(hs_root->add_hs_child(hs_el2) == hr_result_e::hr_success);

    REQUIRE(hs_root->is_child_alive(0, hs_el1) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(1, hs_el1) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(2, hs_el1) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(3, hs_el1) == hr_result_e::hr_true);

    REQUIRE(hs_root->is_child_alive(0, hs_el2) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(1, hs_el2) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(2, hs_el2) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(3, hs_el1) == hr_result_e::hr_true);

    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);
    REQUIRE(std::get<0>(hs_root->push_epoch(std::nullopt, true)) == hr_result_e::hr_success);

    REQUIRE(hs_root->is_child_alive(0, hs_el1) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(1, hs_el1) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(2, hs_el1) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(3, hs_el1) == hr_result_e::hr_true);
    REQUIRE(hs_root->is_child_alive(4, hs_el1) == hr_result_e::hr_true);
    REQUIRE(hs_root->is_child_alive(5, hs_el1) == hr_result_e::hr_true);

    REQUIRE(hs_root->is_child_alive(0, hs_el2) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(1, hs_el2) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(2, hs_el2) == hr_result_e::hr_false);
    REQUIRE(hs_root->is_child_alive(3, hs_el2) == hr_result_e::hr_true);
    REQUIRE(hs_root->is_child_alive(4, hs_el2) == hr_result_e::hr_true);
    REQUIRE(hs_root->is_child_alive(5, hs_el2) == hr_result_e::hr_true);

    REQUIRE(hs_el1->is_unmodified() == true);
    REQUIRE(hs_el2->is_unmodified() == true);
    REQUIRE(hs_root->is_unmodified() == true);

    hs_el1->set_value(10);
    REQUIRE(hs_el1->is_unmodified() == false);
    REQUIRE(hs_el2->is_unmodified() == true);
    REQUIRE(hs_root->is_unmodified() == false);

    REQUIRE(hs_root->reset() == hr_result_e::hr_success);
    REQUIRE(hs_el1->is_unmodified() == true);
    REQUIRE(hs_el2->is_unmodified() == true);
    REQUIRE(hs_root->is_unmodified() == true);

  }

  SECTION ("record test") {

    hist_doc_base_t *hs_root = new hist_doc_base_t;
    hist_doc_base_t *hs_it1 = new hist_doc_base_t;
    hist_doc_t<int> *hs_el1 = new hist_doc_t<int>(0);
    hist_doc_t<int> *hs_el2 = new hist_doc_t<int>(0);

    hs_it1->begin_recording();

    REQUIRE(hs_it1->add_hs_child(hs_el1) == hr_result_e::hr_success);
    REQUIRE(hs_it1->add_hs_child(hs_el2) == hr_result_e::hr_success);

    hs_it1->end_recording();

    REQUIRE(hs_root->add_hs_child(hs_it1) == hr_result_e::hr_success);

  }


}
