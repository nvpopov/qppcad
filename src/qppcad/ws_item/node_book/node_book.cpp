#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

node_book_t::node_book_t() {

  set_default_flags(ws_item_flags_default);
  m_scene = new QGraphicsScene;

}

void node_book_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void node_book_t::render() {
 //do nothing
}

bool node_book_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string node_book_t::compose_type_descr() {
  return "node_book";
}

void node_book_t::update(float delta_time) {
  //do nothing
}

float node_book_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t node_book_t::get_amount_of_selected_content() {
  return 0;
}

size_t node_book_t::get_content_count() {
  return 0;
}

void node_book_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void node_book_t::load_from_json(json &data) {
  ws_item_t::load_from_json(data);
}

void node_book_t::updated_internally() {

}

