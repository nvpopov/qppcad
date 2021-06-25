#include <qppcad/ws_item/py_note_book/py_note_book.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

py_note_book_t::py_note_book_t() : ws_item_t() {
  set_default_flags(ws_item_flags_default);
}

void py_note_book_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                           vector3<float> &out_look_at) {
  //do nothing
}

void py_note_book_t::render() {
  //do nothing
}

bool py_note_book_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string py_note_book_t::compose_type_descr() {
  return "py_note_book";
}

void py_note_book_t::update(float delta_time) {
  //do nothing
}

float py_note_book_t::get_bb_prescaller() {
  return 1.0f;
}

uint32_t py_note_book_t::get_num_cnt_selected() {
  return 0;
}

size_t py_note_book_t::content_count() {
  return 0;
}

void py_note_book_t::save_to_json(json &data) {
  ws_item_t::save_to_json(data);
}

void py_note_book_t::load_from_json(json &data, repair_connection_info_t &rep_info) {
  ws_item_t::load_from_json(data, rep_info);
}

void py_note_book_t::updated_externally(uint32_t update_reason) {

  ws_item_t::updated_externally(update_reason);

}

