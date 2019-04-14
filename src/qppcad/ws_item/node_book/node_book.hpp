#ifndef QPPCAD_NODE_BOOK
#define QPPCAD_NODE_BOOK

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>

namespace qpp {

  namespace cad {

    class node_book_t : public ws_item_t {

        QPP_OBJECT(node_book_t, ws_item_t)

        public:

          node_book_graphics_scene_t *m_scene;

          node_book_t();

          void vote_for_view_vectors(vector3<float> &out_look_pos,
                                     vector3<float> &out_look_at) override ;
          void render() override;
          bool mouse_click(ray_t<float> *click_ray) override;

          std::string compose_type_descr() override;
          void update(float delta_time) override;
          float get_bb_prescaller() override;
          void updated_externally(uint32_t update_reason) override;
          uint32_t get_amount_of_selected_content() override;
          size_t get_content_count() override;

          void save_to_json(json &data) override;
          void load_from_json(json &data, repair_connection_info_t &rep_info) override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
