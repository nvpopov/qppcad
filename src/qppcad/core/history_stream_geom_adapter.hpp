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
struct xgeom_acts_vt {
  using type = std::variant<insert_atom_event_t<REAL>>;
};

template<typename REAL, typename CELL>
class hist_doc_xgeom_proxy_t : public hist_doc_base_t,
                               public geometry_observer<REAL> {

private:

  xgeometry<REAL, CELL> *p_xgeom;
  std::map<epoch_t, typename xgeom_acts_vt<REAL>::type> p_epoch_data;

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

};

}

}

#endif
