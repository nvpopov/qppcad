#ifndef QPP_CAD_WS_ATOMS_LIST_ANYM_SUBSYS
#define QPP_CAD_WS_ATOMS_LIST_ANYM_SUBSYS

#include <geom/geom.hpp>
#include <geom/geom_anim.hpp>
#include <geom/tws_tree.hpp>
#include <qppcad/app.hpp>
#include <vector>
#include <algorithm>

namespace qpp {

  namespace cad {

    template <typename DATA, typename REAL>
    class ws_atoms_list_anim_subsys_t {

      private:

        DATA* p_owner;

      public:

        std::vector<geom_anim_record_t<REAL> >  m_anim_data;
        REAL m_cur_anim_time{0.0f};
        REAL m_anim_frame_time{1.0f};
        int m_cur_anim{0};
        bool m_rebuild_bonds_in_anim{true};
        bool m_play_cyclic{false};
        bool m_play_anim{false};
        bool m_force_non_animable{false};

        ws_atoms_list_anim_subsys_t (DATA &_p_owner);

        void update_geom_to_anim (const int anim_id, const REAL current_frame);

        void update_geom_to_anim ();

        void update (const REAL delta_time);

        bool animable () const;

        void manual_frame_manipulate(const int frame_mod);

        geom_anim_type get_cur_anim_type () const;

        size_t get_total_anims () const;

        size_t frame_count (const size_t anim_id) const;

        size_t current_frame_count () const;
    };

  }

}

#endif
