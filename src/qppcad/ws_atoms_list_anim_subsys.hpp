#ifndef QPP_CAD_WS_ATOMS_LIST_ANYM_SUBSYS
#define QPP_CAD_WS_ATOMS_LIST_ANYM_SUBSYS

#include <geom/geom.hpp>
#include <geom/geom_anim.hpp>
#include <geom/tws_tree.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    class ws_atoms_list_t;

    class ws_atoms_list_anim_subsys_t {

      private:

        ws_atoms_list_t* p_owner;

      public:

        std::vector<geom_anim_record_t<float> >  m_anim_data;
        float m_cur_anim_time{0.0f};
        float m_anim_frame_time{0.25f};
        int m_cur_anim{0};
        bool m_rebuild_bonds_in_anim{true};
        bool m_play_cyclic{true};
        bool m_play_anim{false};
        bool m_force_non_animable{false};

        ws_atoms_list_anim_subsys_t (ws_atoms_list_t &_p_owner);

        void update_geom_to_anim (const int anim_id, const float current_frame);
        void update_and_set_anim (const int anim_id, const float current_frame);
        void update_geom_to_anim ();

        void update (const float delta_time);
        bool animable () const;
        void next_anim();
        void prev_anim();
        void traverse_anim(int travel_dir);

        //void render_ui();

        void manual_frame_manipulate(const int frame_mod);

        geom_anim_record_t<float>* get_current_anim();
        geom_anim_type get_cur_anim_type () const;

        size_t get_total_anims () const;
        size_t frame_count (const size_t anim_id) const;
        size_t current_frame_count () const;
    };

  }

}

#endif
