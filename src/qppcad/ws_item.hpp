#ifndef QPP_WORKSPACE_ITEM_H
#define QPP_WORKSPACE_ITEM_H

#include <qppcad/qppcad.hpp>
#include <geom/lace3d.hpp>
#include <geom/aabb.hpp>
#include <geom/ray.hpp>
#include <qppcad/json_adapter.hpp>
#include <qppcad/qpp_object.hpp>

#include <QPainter>

namespace qpp {

  namespace cad {

    class workspace_t;
    class app_state_t;

    // ws_item_t capabilities
    const uint32_t ws_item_flags_default                 = 0;
    const uint32_t ws_item_flags_support_translation     = 1 << 1;
    const uint32_t ws_item_flags_support_scaling         = 1 << 2;
    const uint32_t ws_item_flags_support_rotation        = 1 << 3;
    const uint32_t ws_item_flags_support_content_editing = 1 << 4;
    const uint32_t ws_item_flags_support_selection       = 1 << 5;
    const uint32_t ws_item_flags_support_rendering_bb    = 1 << 6;
    const uint32_t ws_item_flags_toolbar_extension       = 1 << 7;
    const uint32_t ws_item_flags_support_actions         = 1 << 8;
    const uint32_t ws_item_flags_support_delete          = 1 << 9;
    const uint32_t ws_item_flags_support_clone           = 1 << 10;
    const uint32_t ws_item_flags_support_moveto          = 1 << 11;
    const uint32_t ws_item_flags_support_rendering       = 1 << 12;
    const uint32_t ws_item_flags_support_extended_editor = 1 << 13;

    enum ws_item_tag {
      tag_abstract_item,
      tag_ws_atoms_list,
      tag_ws_comp_chem_data,
      tag_ws_volume_data
    };

    class ws_item_t : public std::enable_shared_from_this<ws_item_t>,
        public qpp_object_t {

        QPP_OBJECT(ws_item_t, qpp_object_t)

      private:

        uint32_t p_flags;

      public:

        ws_item_tag m_tag{ws_item_tag::tag_abstract_item};
        workspace_t *m_parent_ws{};
        std::vector<std::shared_ptr<ws_item_t> > m_connected_items;

        std::string      m_name;
        aabb_3d_t<float> m_aabb;
        vector3<float>   m_pos_old;
        vector3<float>   m_pos{0.0f, 0.0f, 0.0f}; ///
        vector3<float>   m_scale{1.0f, 1.0f, 1.0f}; ///
        vector3<float>   m_rotation{1.0f, 1.0f, 1.0f}; ///
        vector3<float>   explicit_translation;
        int m_last_tab{-1};
        bool             m_is_visible{true}; ///
        bool             m_draw_cell{true}; ///
        bool             m_selected{false}; ///
        bool             m_marked_for_deletion{false};

        /// \brief vote_for_view_vectors
        /// \param vOutLookPos
        /// \param vOutLookAt
        virtual void vote_for_view_vectors(vector3<float> &vOutLookPos,
                                           vector3<float> &vOutLookAt) = 0;

        /// \brief set_parent_workspace
        /// \param _parent_ws
        void set_parent_workspace(workspace_t *_parent_ws);

        /// \brief get_name
        /// \return
        const std::string get_name();

        /// \brief set_name
        /// \param _name
        void set_name(const std::string &_name);

        /// \brief set_name
        /// \param _name
        void set_name(const char * _name);

        void add_connected_item(std::shared_ptr<ws_item_t> new_item);
        void remove_connected_item(std::shared_ptr<ws_item_t> item_to_remove);
        std::optional<size_t> get_connected(std::shared_ptr<ws_item_t> item_to_find);
        bool is_connected_to(std::shared_ptr<ws_item_t> item_to_find);

        bool is_selected();

        /// \brief render
        virtual void render();
        virtual void render_overlay(QPainter &painter);

        virtual std::string get_ws_item_class_name() { return "ws_abstract_item";}
        virtual size_t get_content_count() = 0;
        /// \brief render_ui


        /// \brief mouse_click
        /// \param ray
        /// \return
        virtual bool mouse_click(ray_t<float> *ray) = 0;

        void set_default_flags(uint32_t);

        uint32_t get_flags() const ;

        /// \brief get_amount_of_selected_content
        /// \return
        virtual uint32_t get_amount_of_selected_content() = 0;

        /// \brief compose_item_name
        /// \return
        virtual std::string compose_item_name() = 0;

        /// \brief update
        /// \param delta_time
        virtual void update(float delta_time);

        /// \brief get_bb_prescaller
        /// \return
        virtual float get_bb_prescaller();
        virtual bool is_bb_visible();
        /// \brief get_gizmo_content_barycenter
        /// \return
        virtual const vector3<float> get_gizmo_content_barycenter();

        virtual void on_begin_node_gizmo_translate();
        virtual void on_end_node_gizmo_translate();
        virtual void on_begin_content_gizmo_translate();
        virtual void apply_intermediate_translate_content(const vector3<float> &new_pos);
        virtual void on_end_content_gizmo_translate();
        void translate(vector3<float> tr_vec);
        virtual void save_to_json(json &data);
        virtual void load_from_json(json &data);
        virtual bool can_be_written_to_json();
    };

  }

}

#endif
