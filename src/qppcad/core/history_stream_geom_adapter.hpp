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
struct change_atom_event_t {
  size_t m_atom_idx;
  std::string m_before_aname;
  vector3<REAL> m_before_apos;
  std::string m_after_aname;
  vector3<REAL> m_after_apos;
};

template<typename REAL>
struct change_cell_event_t {
  std::array<std::optional<vector3<REAL>>, 3> new_cell;
  std::array<std::optional<vector3<REAL>>, 3> old_cell;
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

struct change_dim_event_t {
  int new_dim{0};
  int old_dim{0};
};

struct reorder_atoms_event_t {
  std::vector<int> m_atoms_order;
};

struct xfield_changed_event_t {
  size_t m_atom_id;
  size_t m_field_id;
  std::variant<double, float, int, std::string, bool, std::monostate> m_before;
  std::variant<double, float, int, std::string, bool, std::monostate> m_after;
};

template<typename REAL>
struct xgeom_acts_vt {
  using type = std::variant<insert_atom_event_t<REAL>,
                            change_atom_event_t<REAL>,
                            change_cell_event_t<REAL>,
                            reorder_atoms_event_t,
                            change_dim_event_t,
                            erase_atom_event_t<REAL>,
                            select_atoms_event_t,
                            xfield_changed_event_t>;
};

enum xgeom_proxy_hs_act_type_e {
  hs_act_emit_hs_event,
  hs_act_emit_geom_change,
  hs_act_emit_both
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
  std::optional<STRING_EX> p_stored_aname;
  std::optional<vector3<REAL>> p_stored_apos;
  std::variant<double, float, int, std::string, bool, std::monostate> m_xfield_bval;

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

                           [geom_wrp](change_atom_event_t<REAL> &ev) {
                             geom_wrp->change(ev.m_atom_idx, ev.m_before_aname, ev.m_before_apos);
                           },

                           [geom_wrp](erase_atom_event_t<REAL> &ev) {
                             geom_wrp->insert(ev.m_atom_idx, ev.m_atom_name, ev.m_atom_pos);
                           },

                           [geom_wrp](reorder_atoms_event_t &ev) {
                             geom_wrp->reorder(ev.m_atoms_order);
                           },

                           [geom_wrp](change_dim_event_t &ev) {
                             geom_wrp->set_DIM(ev.old_dim);
                           },

                           [geom_wrp](change_cell_event_t<REAL> &ev) {
                             for (size_t i = 0; i < 3; i++)
                               if (geom_wrp->get_DIM() > i && ev.old_cell[i])
                                 geom_wrp->cell.v[i] = *(ev.old_cell[i]);
                           },

                           [geom_wrp](xfield_changed_event_t &ev) {

                             std::vector<STRING_EX> fn;
                             std::vector<basic_types> ft;

                             geom_wrp->get_format(fn, ft);

                             switch (ft[ev.m_field_id]) {
                             case type_int: {
                               geom_wrp->template set_xfield<int>(ev.m_field_id, ev.m_atom_id,
                                                                  std::get<int>(ev.m_before));
                               break;
                             }
                             case type_real: {
                               geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                                     std::get<double>(ev.m_before));
                               break;
                             }
                             case type_double: {
                               geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                                     std::get<double>(ev.m_before));
                               break;
                             }
                             case type_float: {
                               geom_wrp->template set_xfield<float>(ev.m_field_id, ev.m_atom_id,
                                                                    std::get<float>(ev.m_before));
                               break;
                             }
                             case type_bool: {
                               geom_wrp->template set_xfield<bool>(ev.m_field_id, ev.m_atom_id,
                                                                   std::get<bool>(ev.m_before));
                               break;
                             }
                             case type_string: {
                               geom_wrp->template set_xfield<STRING_EX>(
                                   ev.m_field_id, ev.m_atom_id,std::get<STRING_EX>(ev.m_before));
                               break;
                             }
                             }

                           },

                           [geom_wrp](select_atoms_event_t &ev) {},
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

                       [geom_wrp](change_atom_event_t<REAL> &ev) {
                         geom_wrp->change(ev.m_atom_idx, ev.m_after_aname, ev.m_after_apos);
                       },

                       [geom_wrp](erase_atom_event_t<REAL> &ev) {
                         geom_wrp->erase(ev.m_atom_idx);
                       },

                       [geom_wrp](reorder_atoms_event_t &ev) {
                         geom_wrp->reorder(ev.m_atoms_order);
                       },

                       [geom_wrp](change_dim_event_t &ev) {
                         geom_wrp->set_DIM(ev.new_dim);
                       },

                       [geom_wrp](change_cell_event_t<REAL> &ev) {
                         for (size_t i = 0; i < 3; i++)
                           if (geom_wrp->get_DIM() > i && ev.new_cell[i])
                             geom_wrp->cell.v[i] = *(ev.new_cell[i]);
                       },

                       [geom_wrp](xfield_changed_event_t &ev) {

                         std::vector<STRING_EX> fn;
                         std::vector<basic_types> ft;

                         geom_wrp->get_format(fn, ft);

                         switch (ft[ev.m_field_id]) {

                         case type_int: {
                           geom_wrp->template set_xfield<int>(ev.m_field_id, ev.m_atom_id,
                                                              std::get<int>(ev.m_after));
                           break;
                         }
                         case type_real: {
                           geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                                 std::get<double>(ev.m_after));
                           break;
                         }
                         case type_double: {
                           geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                                 std::get<double>(ev.m_after));
                           break;
                         }
                         case type_float: {
                           geom_wrp->template set_xfield<float>(ev.m_field_id, ev.m_atom_id,
                                                                std::get<float>(ev.m_after));
                           break;
                         }
                         case type_bool: {
                           geom_wrp->template set_xfield<bool>(ev.m_field_id, ev.m_atom_id,
                                                               std::get<bool>(ev.m_after));
                           break;
                         }
                         case type_string: {
                           geom_wrp->template set_xfield<STRING_EX>(
                               ev.m_field_id, ev.m_atom_id,std::get<STRING_EX>(ev.m_after));
                           break;
                         }
                         }

                       },

                       [geom_wrp](select_atoms_event_t &ev) {},
                       }, val);

      }

    }

    p_currently_applying_dstate = false;
    return hs_result_e::hs_success;

  }

public:

  void commit_changes(bool new_epoch) {

    if (p_tmp_acts.empty()) return;

    if (new_epoch) {
      auto res = commit_exclusive(nullptr, std::nullopt, false);
      if (res != hs_result_e::hs_success) return;
    }

    epoch_t cur_epoch = get_cur_epoch();
    auto ep_it = p_epoch_data.find(cur_epoch);
    if (ep_it == end(p_epoch_data)) {
      p_epoch_data[cur_epoch] = p_tmp_acts;
    } else {
      std::copy(begin(p_tmp_acts), end(p_tmp_acts), std::back_inserter(ep_it->second));
    }

    p_tmp_acts.clear();

  }

  uint32_t get_flags() override {
    return geometry_observer_supports_default
           | geometry_observer_supports_add
           | geometry_observer_supports_insert
           | geometry_observer_supports_change
           | geometry_observer_supports_erase
           | geometry_observer_supports_shadow
           | geometry_observer_supports_reorder
           | geometry_observer_supports_xfield_change;
  };

  void added(before_after order,
             const STRING_EX &aname,
             const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate) return;
    if (order == before_after::before) return;
    insert_atom_event_t<REAL> insert_atom_event;
    insert_atom_event.m_atom_name = aname;
    insert_atom_event.m_atom_pos = apos;
    p_tmp_acts.push_back(std::move(insert_atom_event));

    if (!p_currently_editing) commit_changes(true);

  }

  void inserted(int at, before_after order,
                const STRING_EX &aname,
                const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate) return;

    if (order == before_after::before) return;

    insert_atom_event_t<REAL> insert_atom_event;
    insert_atom_event.m_atom_name = aname;
    insert_atom_event.m_atom_pos = apos;
    insert_atom_event.m_atom_idx = std::optional<size_t>(at);
    p_tmp_acts.push_back(std::move(insert_atom_event));

    if (!p_currently_editing) commit_changes(true);

  }

  void changed(int at, before_after order,
               const STRING_EX &aname,
               const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate) return;

    if (order == before_after::before) {
      p_stored_aname = p_xgeom->atom_name(at);
      p_stored_apos = p_xgeom->pos(at);
    } else {

      if (p_stored_apos && p_stored_aname) {

        //std::cout << "CHANGED " << aname << " " << apos << std::endl;

        change_atom_event_t<REAL> change_atom_event;
        change_atom_event.m_atom_idx = at;
        change_atom_event.m_before_aname = *p_stored_aname;
        change_atom_event.m_before_apos = *p_stored_apos;
        change_atom_event.m_after_aname = aname;
        change_atom_event.m_after_apos = apos;
        p_tmp_acts.push_back(std::move(change_atom_event));

        if (!p_currently_editing) commit_changes(true);

      }

      p_stored_apos = std::nullopt;
      p_stored_aname = std::nullopt;

    }

  }

  void erased(int at, before_after order) override {

    if (p_currently_applying_dstate) return;

    if (order == before_after::after) return;

    erase_atom_event_t<REAL> erase_atom_event;
    //std::cout << "@@@ " << at << std::endl;
    erase_atom_event.m_atom_idx = at;
    erase_atom_event.m_atom_pos = p_xgeom->pos(at);
    erase_atom_event.m_atom_name = p_xgeom->atom(at);
    p_tmp_acts.push_back(std::move(erase_atom_event));

    if (!p_currently_editing) commit_changes(true);

  }

  void shaded(int at, before_after, bool) override {

    if (p_currently_applying_dstate) return;

  }

  void reordered (const std::vector<int> &ord, before_after order) override {

    if (p_currently_applying_dstate) return;
    if (order == before_after::after) return;

    reorder_atoms_event_t reorder_atoms_event;
    reorder_atoms_event.m_atoms_order = ord;
    p_tmp_acts.push_back(std::move(reorder_atoms_event));

    if (!p_currently_editing) commit_changes(true);

  }

  void geometry_destroyed () override {

  }


  void dim_changed(before_after ord) override {

  }

  void cell_changed(before_after ord) override {

  }

  void xfield_changed(int xid, int at, before_after ord) override {

    if (p_currently_applying_dstate) return;

    std::vector<STRING_EX> fn;
    std::vector<basic_types> ft;

    p_xgeom->get_format(fn, ft);
    //std::cout << "@@@XFIELD_CHANGED size(fn) " << size(fn) << " " << size(ft) << std::endl;

    if (ord == before_after::before) {

      switch (ft[xid]) {
       case type_int:{m_xfield_bval = p_xgeom->template get_xfield<int>(xid, at); break;}
       case type_real:{m_xfield_bval = p_xgeom->template get_xfield<double>(xid, at); break;}
       case type_double:{m_xfield_bval = p_xgeom->template get_xfield<double>(xid, at); break;}
       case type_float:{m_xfield_bval = p_xgeom->template get_xfield<float>(xid, at); break;}
       case type_bool:{m_xfield_bval = p_xgeom->template get_xfield<bool>(xid, at); break;}
       case type_string:{m_xfield_bval = p_xgeom->template get_xfield<STRING_EX>(xid, at); break;}
      }

    } else /*after*/ {

      xfield_changed_event_t xch_ev;
      xch_ev.m_atom_id = at;
      xch_ev.m_field_id = xid;
      xch_ev.m_before = m_xfield_bval;
      switch (ft[xid]) {
       case type_int:{xch_ev.m_after = p_xgeom->template get_xfield<int>(xid, at);break;}
       case type_real:{xch_ev.m_after = p_xgeom->template get_xfield<double>(xid, at); break;}
       case type_double:{xch_ev.m_after = p_xgeom->template get_xfield<double>(xid, at); break;}
       case type_float:{xch_ev.m_after = p_xgeom->template get_xfield<float>(xid, at); break;}
       case type_bool:{xch_ev.m_after = p_xgeom->template get_xfield<bool>(xid, at); break;}
       case type_string:{xch_ev.m_after = p_xgeom->template get_xfield<STRING_EX>(xid, at); break;}
      }

      p_tmp_acts.push_back(std::move(xch_ev));

    }

    if (!p_currently_editing) commit_changes(true);

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

  void end_editing(bool new_epoch = true) {

    assert(p_currently_editing);
    commit_changes(new_epoch);
    p_currently_editing = false;

  }

  //explicit editing methods

  void hs_change_DIM(xgeom_proxy_hs_act_type_e evtype, int newdim) {

    if (!p_xgeom) return;

    if (evtype == hs_act_emit_hs_event || evtype == hs_act_emit_both) {
      change_dim_event_t change_dim_event;
      change_dim_event.old_dim = p_xgeom->get_DIM();
      change_dim_event.new_dim = newdim;
      p_tmp_acts.push_back(std::move(change_dim_event));
    }

    if (evtype == hs_act_emit_geom_change || evtype == hs_act_emit_both) {
      p_xgeom->set_DIM(newdim);
      //p_xgeom->cell.DIM = newdim;
    }

    if (!p_currently_editing) commit_changes(true);

  }

  void hs_change_cell(xgeom_proxy_hs_act_type_e evtype,
                      std::optional<vector3<REAL>> a,
                      std::optional<vector3<REAL>> b = std::nullopt,
                      std::optional<vector3<REAL>> c = std::nullopt) {

    if (!p_xgeom) return;

    if (evtype == hs_act_emit_hs_event || evtype == hs_act_emit_both) {
      change_cell_event_t<REAL> change_cell_event;
      //old cell
      if (p_xgeom->get_DIM() > 0) change_cell_event.old_cell[0] = p_xgeom->cell.v[0];
      if (p_xgeom->get_DIM() > 1) change_cell_event.old_cell[1] = p_xgeom->cell.v[1];
      if (p_xgeom->get_DIM() > 2) change_cell_event.old_cell[2] = p_xgeom->cell.v[2];
      //new cell
      if (a && p_xgeom->get_DIM() > 0) change_cell_event.new_cell[0] = *a;
      if (b && p_xgeom->get_DIM() > 0) change_cell_event.new_cell[1] = *b;
      if (c && p_xgeom->get_DIM() > 0) change_cell_event.new_cell[2] = *c;
      p_tmp_acts.push_back(std::move(change_cell_event));
    }

    if (evtype == hs_act_emit_geom_change || evtype == hs_act_emit_both) {
      if (a && p_xgeom->get_DIM() > 0) p_xgeom->cell.v[0] = *a;
      if (b && p_xgeom->get_DIM() > 1) p_xgeom->cell.v[1] = *b;
      if (c && p_xgeom->get_DIM() > 2) p_xgeom->cell.v[2] = *c;
    }

    if (!p_currently_editing) commit_changes(true);

  }

};

}

}

#endif
