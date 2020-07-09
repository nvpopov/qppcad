#ifndef HISTORY_STREAM_XGEOM_PROXY
#define HISTORY_STREAM_XGEOM_PROXY

#include <qppcad/core/history_stream.hpp>
#include <geom/xgeom.hpp>
#include <cassert>
#include <variant>

namespace qpp {

namespace cad {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

template<typename REAL>
struct insert_atom_event_t {
  std::optional<size_t> m_atom_idx;
  std::string m_atom_name;
  vector3<REAL> m_atom_pos;
};

template<typename REAL>
struct change_cell_event_t {
  std::vector<vector3<REAL>> new_cell;
  std::vector<vector3<REAL>> old_cell;
};

template<typename REAL>
struct erase_atom_event_t {
  size_t m_atom_idx;
  std::string m_atom_name;
  vector3<REAL> m_atom_pos;
};

struct select_atoms_event_t {
  std::vector<std::tuple<size_t, index>> sel_atoms;
};

template<typename REAL>
struct xgeom_acts_vt {
  using type = std::variant<insert_atom_event_t<REAL>,
                            change_cell_event_t<REAL>,
                            erase_atom_event_t<REAL>,
                            select_atoms_event_t>;
};

template<typename REAL, typename CELL>
class hist_doc_xgeom_proxy_t : public hs_doc_base_t,
                               public geometry_observer<REAL> {

private:

  using acts_t = typename xgeom_acts_vt<REAL>::type;
  xgeometry<REAL, CELL> *p_xgeom{};
  std::map<epoch_t, std::vector<acts_t>> p_epoch_data;
  std::vector<acts_t> p_tmp_acts;
  bool p_currently_editing;
  bool p_currently_applying_dstate{false};

protected:

  hs_result_e dstate_change(hs_dstate_apply_e ds_dir, epoch_t target) override {

    p_currently_applying_dstate = true;

    xgeometry<REAL, CELL> *geom_wrp = p_xgeom;
    if (!geom_wrp) {
      p_currently_applying_dstate = false;
      return hs_result_e::hs_error;
    }

    auto epoch_it = p_epoch_data.find(target);
    if (epoch_it == end(p_epoch_data)) {
      p_currently_applying_dstate = false;
      return hs_result_e::hs_invalid_epoch;
    }

    auto &acts = p_epoch_data[target];

    if (ds_dir == hs_ds_unapply) {

      for (auto i = rbegin(acts); i != rend(acts); ++i) {

            auto val = *i;

            std::visit(overloaded {
                           [geom_wrp](auto arg) {},
                           [geom_wrp](insert_atom_event_t<REAL> &ev) {
                             geom_wrp->erase(ev.m_atom_idx.value_or(geom_wrp->nat()-1));
                             },
                           [geom_wrp](change_cell_event_t<REAL> &event) {

                           },
                           [geom_wrp](select_atoms_event_t &event) {},
                           }, val);

      }

    } else /*ds_dir == hs_ds_apply*/ {

      for (auto i = begin(acts); i != end(acts); ++i) {

        auto val = *i;

        std::visit(overloaded {
                       [geom_wrp](auto arg) {},
                       [geom_wrp](insert_atom_event_t<REAL> &ev) {
                         if (ev.m_atom_idx) {
                           geom_wrp->insert(*(ev.m_atom_idx), ev.m_atom_name, ev.m_atom_pos);
                         } else {
                           geom_wrp->add(ev.m_atom_name, ev.m_atom_pos);
                         }
                         },
                       [geom_wrp](change_cell_event_t<REAL> &ev) {},
                       [geom_wrp](select_atoms_event_t &ev) {},
                       }, val);
      }

    }

    p_currently_applying_dstate = false;
    return hs_result_e::hs_success;

  }

public:

  void added(before_after order, const STRING_EX &aname, const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate) return;
    if (order == before_after::before) return;
    insert_atom_event_t<REAL> insert_atom_event;
    insert_atom_event.m_atom_name = aname;
    insert_atom_event.m_atom_pos = apos;
    p_tmp_acts.push_back(std::move(insert_atom_event));

  }

  void inserted(int at, before_after, const STRING_EX &, const vector3<REAL> &) override {

    if (p_currently_applying_dstate) return;

  }

  void changed(int at, before_after, const STRING_EX &, const vector3<REAL> &) override {

    if (p_currently_applying_dstate) return;

  }

  void erased(int at, before_after order) override {

    if (p_currently_applying_dstate) return;
    if (order == before_after::before) return;
    erase_atom_event_t<REAL> erase_atom_event;
    erase_atom_event.m_atom_idx = at;
    p_tmp_acts.push_back(std::move(erase_atom_event));

  }

  void shaded(int at, before_after, bool) override {

    if (p_currently_applying_dstate) return;

  }

  void reordered (const std::vector<int> &, before_after) override {

    if (p_currently_applying_dstate) return;

  }

  void geometry_destroyed () override {

  }

  void set_xgeom(xgeometry<REAL, CELL> *xgeom) {
    if (xgeom) {
      p_xgeom = xgeom;
      p_xgeom->add_observer(*this);
    }
  }

  xgeometry<REAL, CELL> *get_xgeom() {
    return p_xgeom;
  }

  hist_doc_xgeom_proxy_t() {
    set_dstate_type(hs_dstate_incr);
  }

  void begin_editing() {
    p_currently_editing = true;
  }

  void end_editing() {

    assert(p_currently_editing);

    auto res = commit_exclusive(nullptr, std::nullopt, false);
    if (!res) return;
    epoch_t cur_epoch = get_cur_epoch();
    p_epoch_data[cur_epoch] = p_tmp_acts;
    p_tmp_acts.clear();

  }

};

}

}

#endif
