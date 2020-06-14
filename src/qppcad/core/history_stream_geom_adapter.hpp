#ifndef HISTORY_STREAM_XGEOM_PROXY
#define HISTORY_STREAM_XGEOM_PROXY

#include <qppcad/core/history_stream.hpp>
#include <geom/xgeom.hpp>
#include <cassert>
#include <variant>

namespace qpp {

namespace cad {

template<typename REAL>
struct insert_atom_event_t {
  size_t m_atom_idx;
  std::string m_atom_name;
  vector3<REAL> m_atom_pos;
};

template<typename REAL>
struct change_cell_event_t {
  std::vector<vector3<REAL>> new_cell;
  std::vector<vector3<REAL>> old_cell;
};

struct select_atoms_event_t {
  std::vector<std::tuple<size_t, index>> sel_atoms;
};

template<typename REAL>
struct xgeom_acts_vt {
  using type = std::variant<insert_atom_event_t<REAL>,
                            change_cell_event_t<REAL>,
                            select_atoms_event_t>;
};

template<typename REAL, typename CELL>
class hist_doc_xgeom_proxy_t : public hist_doc_base_t,
                               public geometry_observer<REAL> {

private:

  using acts_t = typename xgeom_acts_vt<REAL>::type;
  xgeometry<REAL, CELL> *p_xgeom{};
  std::map<epoch_t, std::vector<acts_t>> p_epoch_data;

public:

  void added(before_after, const STRING_EX &, const vector3<REAL> &) override {

  }

  void inserted(int at, before_after, const STRING_EX &, const vector3<REAL> &) override {

  }

  void changed(int at, before_after, const STRING_EX &, const vector3<REAL> &) override {

  }

  void erased(int at, before_after) override {

  }

  void shaded(int at, before_after, bool) override {

  }

  void reordered (const std::vector<int> &, before_after) override {

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

};

}

}

#endif
