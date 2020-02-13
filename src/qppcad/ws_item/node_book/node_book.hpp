#ifndef QPPCAD_NODE_BOOK
#define QPPCAD_NODE_BOOK

#pragma push_macro("slots")
#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#pragma pop_macro("slots")

namespace py = pybind11;

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>
#include <qppcad/sflow/sflow.hpp>

namespace qpp {

  namespace cad {

    class node_book_t final: public ws_item_t {

        QPP_OBJECT(node_book_t, ws_item_t)

      public:

        node_book_graphics_scene_t *m_scene;
        std::shared_ptr<sflow_context_t> m_sflow_context;

        bool m_highlight_dirty_nodes{false};
        bool m_auto_recompute{true};

        node_book_t();

        void vote_for_view_vectors(vector3<float> &out_look_pos,
                                   vector3<float> &out_look_at) override ;
        void render() override;
        bool mouse_click(ray_t<float> *click_ray) override;

        std::string compose_type_descr() override;
        void update(float delta_time) override;
        float get_bb_prescaller() override;
        void updated_externally(uint32_t update_reason) override;
        uint32_t get_num_cnt_selected() override;
        size_t get_content_count() override;

        void save_to_json(json &data) override;
        void load_from_json(json &data, repair_connection_info_t &rep_info) override;
        bool can_be_written_to_json() override;

        void execute();
        void post_execute();
        void update_output_values();

        py::list py_get_visible_rect();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
