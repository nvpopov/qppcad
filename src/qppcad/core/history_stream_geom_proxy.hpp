#ifndef HISTORY_STREAM_XGEOM_PROXY
#define HISTORY_STREAM_XGEOM_PROXY

#include <qppcad/core/history_stream.hpp>
#include <geom/xgeom.hpp>
#include <symm/index_set.hpp>
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
struct change_atom_pos_event_t {
  size_t m_atom_idx;
  vector3<REAL> m_before_apos;
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

struct select_atom_event_t {
  atom_index_set_key m_atom;
  std::optional<size_t> idx;
  bool m_state{true};
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
                            change_atom_pos_event_t<REAL>,
                            change_cell_event_t<REAL>,
                            reorder_atoms_event_t,
                            change_dim_event_t,
                            erase_atom_event_t<REAL>,
                            select_atom_event_t,
                            xfield_changed_event_t>;
};

enum class xgeom_proxy_hs_act_type_e {
  hs_act_emit_hs_event,
  hs_act_emit_geom_change,
  hs_act_emit_both
};

template<typename REAL, typename CELL>
class hist_doc_xgeom_proxy_t : public hs_doc_base_t, public geometry_observer<REAL> {

public:

  using acts_t = typename xgeom_acts_vt<REAL>::type;

private:

  xgeometry<REAL, CELL> *p_xgeom{};
  std::map<epoch_t, std::vector<acts_t>> p_epoch_data;
  std::vector<acts_t> p_cur_acts;
  std::vector<acts_t> p_tmp_acts;
  bool p_currently_applying_dstate{false};
  std::optional<STRING_EX> p_stored_aname;
  std::optional<vector3<REAL>> p_stored_apos;
  std::variant<double, float, int, std::string, bool, std::monostate> m_xfield_bval;

protected:

  void on_begin_recording() override {

  };

  void on_recording() override {

  };

  void on_end_recording() override {

    auto cur_rec_type = get_cur_rec_type();

    switch (cur_rec_type) {

    case hs_doc_rec_type_e::hs_doc_rec_init : {
      init_base_epoch();
    }

    case hs_doc_rec_type_e::hs_doc_rec_init_local : {

    }

    case hs_doc_rec_type_e::hs_doc_rec_as_new_epoch : {
      commit_changes(true);
    }

    default: {

    }

    }

  };

  void apply_action(acts_t &act) {

    xgeometry<REAL, CELL> *geom_wrp = p_xgeom;

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

                   [geom_wrp](change_atom_pos_event_t<REAL> &ev) {
                     geom_wrp->change_pos(ev.m_atom_idx, ev.m_after_apos);
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

                     case basic_types::type_int: {
                       geom_wrp->template set_xfield<int>(ev.m_field_id, ev.m_atom_id,
                                                          std::get<int>(ev.m_after));
                       break;
                     }
                     case basic_types::type_real: {
                       geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                             std::get<double>(ev.m_after));
                       break;
                     }
                     case basic_types::type_double: {
                       geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                             std::get<double>(ev.m_after));
                       break;
                     }
                     case basic_types::type_float: {
                       geom_wrp->template set_xfield<float>(ev.m_field_id, ev.m_atom_id,
                                                            std::get<float>(ev.m_after));
                       break;
                     }
                     case basic_types::type_bool: {
                       geom_wrp->template set_xfield<bool>(ev.m_field_id, ev.m_atom_id,
                                                           std::get<bool>(ev.m_after));
                       break;
                     }
                     case basic_types::type_string: {
                       geom_wrp->template set_xfield<STRING_EX>(
                           ev.m_field_id, ev.m_atom_id,std::get<STRING_EX>(ev.m_after));
                       break;
                     }
                     }

                   },

                   [geom_wrp](select_atom_event_t &ev) {
                     geom_wrp->iselect(ev.m_atom.m_atm, ev.m_atom.m_idx, ev.m_state,
                                       std::nullopt,
                                       false);
                   }
                   }, act);

  }

  void unapply_action(acts_t &act) {

    xgeometry<REAL, CELL> *geom_wrp = p_xgeom;

    std::visit(overloaded {

                   [geom_wrp](auto arg) {},

                   [geom_wrp](insert_atom_event_t<REAL> &ev) {
                     geom_wrp->erase(ev.m_atom_idx.value_or(geom_wrp->nat()-1));
                   },

                   [geom_wrp](change_atom_event_t<REAL> &ev) {
                     geom_wrp->change(ev.m_atom_idx, ev.m_before_aname, ev.m_before_apos);
                   },

                   [geom_wrp](change_atom_pos_event_t<REAL> &ev) {
                     geom_wrp->change_pos(ev.m_atom_idx, ev.m_before_apos);
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
                     case basic_types::type_int: {
                       geom_wrp->template set_xfield<int>(ev.m_field_id, ev.m_atom_id,
                                                          std::get<int>(ev.m_before));
                       break;
                     }
                     case basic_types::type_real: {
                       geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                             std::get<double>(ev.m_before));
                       break;
                     }
                     case basic_types::type_double: {
                       geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
                                                             std::get<double>(ev.m_before));
                       break;
                     }
                     case basic_types::type_float: {
                       geom_wrp->template set_xfield<float>(ev.m_field_id, ev.m_atom_id,
                                                            std::get<float>(ev.m_before));
                       break;
                     }
                     case basic_types::type_bool: {
                       geom_wrp->template set_xfield<bool>(ev.m_field_id, ev.m_atom_id,
                                                           std::get<bool>(ev.m_before));
                       break;
                     }
                     case basic_types::type_string: {
                       geom_wrp->template set_xfield<STRING_EX>(
                           ev.m_field_id, ev.m_atom_id,std::get<STRING_EX>(ev.m_before));
                       break;
                     }
                     }

                   },

                   [geom_wrp](select_atom_event_t &ev) {
                     geom_wrp->iselect(ev.m_atom.m_atm, ev.m_atom.m_idx, !ev.m_state,
                                       std::nullopt,
                                       false);
                   },
                   }, act);

  }

  void process_acts(std::vector<acts_t> &acts, hs_dstate_apply_e ds_dir) {

    p_currently_applying_dstate = true;

    if (ds_dir == hs_dstate_apply_e::hs_ds_unapply) {
      for (auto i = rbegin(acts); i != rend(acts); ++i) unapply_action(*i);
    } else { /*ds_dir == hs_ds_apply*/
      for (auto i = begin(acts); i != end(acts); ++i) apply_action(*i);
    }

    p_currently_applying_dstate = false;

  }

  hs_result_e dstate_change(hs_dstate_apply_e ds_dir, epoch_t target) override {

    p_currently_applying_dstate = true;

    if (!p_xgeom) {
      p_currently_applying_dstate = false;
      return hs_result_e::hs_error;
    }

    auto epoch_it = p_epoch_data.find(target);
    if (epoch_it == end(p_epoch_data)) {
      p_currently_applying_dstate = false;
      return hs_result_e::hs_invalid_epoch;
    }

    auto &acts = p_epoch_data[target];
    process_acts(acts, ds_dir);

    return hs_result_e::hs_success;

  }

public:

  void commit_changes(bool new_epoch, bool ignore_empty_cur_acts = false) {

    if (!ignore_empty_cur_acts && p_cur_acts.empty())
      return;

    if (new_epoch) {
      auto res = commit_exclusive(nullptr, std::nullopt, false);
      if (res != hs_result_e::hs_success)
        return;
    }

    epoch_t cur_epoch = get_cur_epoch();
    auto ep_it = p_epoch_data.find(cur_epoch);
    if (ep_it == end(p_epoch_data)) {
      p_epoch_data[cur_epoch] = p_cur_acts;
    } else {
      std::copy(begin(p_cur_acts), end(p_cur_acts), std::back_inserter(ep_it->second));
    }

    p_cur_acts.clear();

  }

  void commit_changes_external(std::vector<acts_t> &acts, bool new_epoch, bool clear_tmp) {

    if (clear_tmp)
      p_cur_acts.clear();

    std::copy(begin(acts), end(acts), std::back_inserter(p_cur_acts));
    commit_changes(new_epoch);

  }

  uint32_t get_flags() override {
    return geometry_observer_supports_default
           | geometry_observer_supports_add
           | geometry_observer_supports_insert
           | geometry_observer_supports_change
           | geometry_observer_supports_erase
           | geometry_observer_supports_shadow
           | geometry_observer_supports_reorder
           | geometry_observer_supports_select
           | geometry_observer_supports_xfield_change;
  };

  void added(before_after order, const STRING_EX &aname, const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

    if (order == before_after::before)
      return;

    insert_atom_event_t<REAL> insert_atom_event;
    insert_atom_event.m_atom_name = aname;
    insert_atom_event.m_atom_pos = apos;
    p_cur_acts.push_back(std::move(insert_atom_event));

    if (!get_is_recording())
      commit_changes(true);

  }

  void inserted(int at, before_after order, const STRING_EX &aname,
                const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

    if (order == before_after::before)
      return;

    insert_atom_event_t<REAL> insert_atom_event;
    insert_atom_event.m_atom_name = aname;
    insert_atom_event.m_atom_pos = apos;
    insert_atom_event.m_atom_idx = std::optional<size_t>(at);
    p_cur_acts.push_back(std::move(insert_atom_event));

    if (!get_is_recording())
      commit_changes(true);

  }

  void changed(int at, before_after order, const STRING_EX &aname,
               const vector3<REAL> &apos) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

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
        p_cur_acts.push_back(std::move(change_atom_event));

        if (!get_is_recording())
          commit_changes(true);

      }

      p_stored_apos = std::nullopt;
      p_stored_aname = std::nullopt;

    }

  }

  void erased(int at, before_after order) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

    if (order == before_after::after)
      return;

    erase_atom_event_t<REAL> erase_atom_event;
    //std::cout << "@@@ " << at << std::endl;
    erase_atom_event.m_atom_idx = at;
    erase_atom_event.m_atom_pos = p_xgeom->pos(at);
    erase_atom_event.m_atom_name = p_xgeom->atom(at);
    p_cur_acts.push_back(std::move(erase_atom_event));

    if (!get_is_recording())
      commit_changes(true);

  }

  void shaded(int at, before_after, bool) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

  }

  void reordered (const std::vector<int> &ord, before_after order) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

    if (order == before_after::after)
      return;

    reorder_atoms_event_t reorder_atoms_event;
    reorder_atoms_event.m_atoms_order = ord;
    p_cur_acts.push_back(std::move(reorder_atoms_event));

    if (!get_is_recording())
      commit_changes(true);

  }

  void dim_changed(before_after ord) override {

  }

  void cell_changed(before_after ord) override {

  }

  void xfield_changed(int xid, int at, before_after ord) override {

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

    std::vector<STRING_EX> fn;
    std::vector<basic_types> ft;

    p_xgeom->get_format(fn, ft);

    if (ord == before_after::before) {

      switch (ft[xid]) {
       case basic_types::type_int: {
         m_xfield_bval = p_xgeom->template get_xfield<int>(xid, at); break;
       }
       case basic_types::type_real: {
         m_xfield_bval = p_xgeom->template get_xfield<double>(xid, at); break;
       }
       case basic_types::type_double: {
         m_xfield_bval = p_xgeom->template get_xfield<double>(xid, at); break;
       }
       case basic_types::type_float: {
         m_xfield_bval = p_xgeom->template get_xfield<float>(xid, at); break;
       }
       case basic_types::type_bool: {
         m_xfield_bval = p_xgeom->template get_xfield<bool>(xid, at); break;
       }
       case basic_types::type_string: {
         m_xfield_bval = p_xgeom->template get_xfield<STRING_EX>(xid, at); break;
       }
      }

    } else /*after*/ {

      xfield_changed_event_t xch_ev;
      xch_ev.m_atom_id = at;
      xch_ev.m_field_id = xid;
      xch_ev.m_before = m_xfield_bval;
      switch (ft[xid]) {
       case basic_types::type_int: {
         xch_ev.m_after = p_xgeom->template get_xfield<int>(xid, at);break;
       }
       case basic_types::type_real: {
         xch_ev.m_after = p_xgeom->template get_xfield<double>(xid, at); break;
       }
       case basic_types::type_double: {
         xch_ev.m_after = p_xgeom->template get_xfield<double>(xid, at); break;
       }
       case basic_types::type_float: {
         xch_ev.m_after = p_xgeom->template get_xfield<float>(xid, at); break;
       }
       case basic_types::type_bool: {
         xch_ev.m_after = p_xgeom->template get_xfield<bool>(xid, at); break;
       }
       case basic_types::type_string: {
         xch_ev.m_after = p_xgeom->template get_xfield<STRING_EX>(xid, at); break;
       }
      }

      p_cur_acts.push_back(std::move(xch_ev));

    }

    if (!get_is_recording())
      commit_changes(true);

  }

  void selected(atom_index_set_key &sel_at, before_after ord, bool state) override {

//    std::cout << fmt::format("@@@ SELECTED nth={}, ord={}, state={}", nth, ord, state)
//              << std::endl;

    if (p_currently_applying_dstate || get_ignore_changes())
      return;

    if (ord == before_after::before) {
      select_atom_event_t ev;
      ev.m_atom = sel_at;
      ev.m_state = state;
      p_cur_acts.push_back(std::move(ev));
    }

  };

  void set_xgeom(xgeometry<REAL, CELL> *xgeom) {

    if (xgeom) {
      p_xgeom = xgeom;
      p_xgeom->add_observer(*this);
    }

  }

  void modify_epoch(acts_t &&act, epoch_t epoch) {

    auto epoch_it = p_epoch_data.find(epoch);

    if (epoch_it == end(p_epoch_data))
      return;

    p_epoch_data[epoch].push_back(act);

  }

  xgeometry<REAL, CELL> *get_xgeom() {
    return p_xgeom;
  }

  hist_doc_xgeom_proxy_t() {
    set_dstate_type(hs_dstate_e::hs_dstate_incr);
  }

  //explicit editing methods

  void hs_change_DIM(xgeom_proxy_hs_act_type_e evtype, int newdim) {

    if (!p_xgeom)
      return;

    if (evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_hs_event
        || evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_both) {
      change_dim_event_t change_dim_event;
      change_dim_event.old_dim = p_xgeom->get_DIM();
      change_dim_event.new_dim = newdim;
      p_cur_acts.push_back(std::move(change_dim_event));
    }

    if (evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_geom_change
        || evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_both) {
      p_xgeom->set_DIM(newdim);
      //p_xgeom->cell.DIM = newdim;
    }

    if (!get_is_recording())
      commit_changes(true);

  }

  void hs_change_cell(xgeom_proxy_hs_act_type_e evtype,
                      std::optional<vector3<REAL>> a,
                      std::optional<vector3<REAL>> b = std::nullopt,
                      std::optional<vector3<REAL>> c = std::nullopt) {

    if (!p_xgeom) return;

    if (evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_hs_event
        || evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_both) {

      change_cell_event_t<REAL> change_cell_event;
      //old cell
      if (p_xgeom->get_DIM() > 0)
        change_cell_event.old_cell[0] = p_xgeom->cell.v[0];

      if (p_xgeom->get_DIM() > 1)
        change_cell_event.old_cell[1] = p_xgeom->cell.v[1];

      if (p_xgeom->get_DIM() > 2)
        change_cell_event.old_cell[2] = p_xgeom->cell.v[2];

      //new cell
      if (a && p_xgeom->get_DIM() > 0)
        change_cell_event.new_cell[0] = *a;

      if (b && p_xgeom->get_DIM() > 0)
        change_cell_event.new_cell[1] = *b;

      if (c && p_xgeom->get_DIM() > 0)
        change_cell_event.new_cell[2] = *c;

      p_cur_acts.push_back(std::move(change_cell_event));

    }

    if (evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_geom_change
        || evtype == xgeom_proxy_hs_act_type_e::hs_act_emit_both) {

      if (a && p_xgeom->get_DIM() > 0)
        p_xgeom->cell.v[0] = *a;

      if (b && p_xgeom->get_DIM() > 1)
        p_xgeom->cell.v[1] = *b;

      if (c && p_xgeom->get_DIM() > 2)
        p_xgeom->cell.v[2] = *c;

    }

    if (!get_is_recording()) commit_changes(true);

  }

  void init_base_epoch() {

    assert(p_xgeom != nullptr);

    //epoch_t cur_epoch = get_cur_epoch();
    //assert(cur_epoch == 0);

    //clean old data
    p_epoch_data.clear();
    std::vector<acts_t> &acts = p_epoch_data[0];
    clear_epoch_data();

    //fill dim
    change_dim_event_t change_dim_ev;
    change_dim_ev.new_dim = p_xgeom->get_DIM();
    change_dim_ev.old_dim = change_dim_ev.new_dim;
    acts.push_back(std::move(change_dim_ev));

    //fill cell
    change_cell_event_t<REAL> change_cell_ev;
    for (int i = 0; i < p_xgeom->get_DIM(); i++) {
      change_cell_ev.new_cell[i] = p_xgeom->cell.v[i];
      change_cell_ev.old_cell[i] = p_xgeom->cell.v[i];
    }
    acts.push_back(std::move(change_cell_ev));

    //fill atoms
    for (int i = 0; i < p_xgeom->nat(); i++) {
      insert_atom_event_t<REAL> insert_atom_ev;
      insert_atom_ev.m_atom_name = p_xgeom->atom_name(i);
      insert_atom_ev.m_atom_pos = p_xgeom->pos(i);
      acts.push_back(std::move(insert_atom_ev));
    }

  }

  void set_tmp_acts(std::vector<acts_t> &&new_tmp_acts) {
    p_tmp_acts = new_tmp_acts;
  }

  std::vector<acts_t> &get_tmp_acts() {
    return p_tmp_acts;
  }

  void revert_tmp_acts() {
    process_acts(p_tmp_acts, hs_dstate_apply_e::hs_ds_unapply);
  }

  void make_tmp_epoch_permanent() {

    xgeometry<REAL, CELL> *geom_wrp = p_xgeom;

    for (auto &act : p_tmp_acts) {

      std::visit(overloaded {

                     [geom_wrp](auto arg) {},

                     [geom_wrp](insert_atom_event_t<REAL> &ev) {
                       //geom_wrp->erase(ev.m_atom_idx.value_or(geom_wrp->nat()-1));
                     },

                     [geom_wrp](change_atom_event_t<REAL> &ev) {
                       //geom_wrp->change(ev.m_atom_idx, ev.m_before_aname, ev.m_before_apos);
                     },

                     [geom_wrp](change_atom_pos_event_t<REAL> &ev) {
                       //geom_wrp->change_pos(ev.m_atom_idx, ev.m_before_apos);
                       ev.m_after_apos = geom_wrp->pos(ev.m_atom_idx);
                     },

                     [geom_wrp](erase_atom_event_t<REAL> &ev) {
                       //geom_wrp->insert(ev.m_atom_idx, ev.m_atom_name, ev.m_atom_pos);
                     },

                     [geom_wrp](reorder_atoms_event_t &ev) {
                       //geom_wrp->reorder(ev.m_atoms_order);
                     },

                     [geom_wrp](change_dim_event_t &ev) {
                       //geom_wrp->set_DIM(ev.old_dim);
                     },

                     [geom_wrp](change_cell_event_t<REAL> &ev) {
//                       for (size_t i = 0; i < 3; i++)
//                         if (geom_wrp->get_DIM() > i && ev.old_cell[i])
//                           geom_wrp->cell.v[i] = *(ev.old_cell[i]);
                     },

                     [geom_wrp](xfield_changed_event_t &ev) {

//                       std::vector<STRING_EX> fn;
//                       std::vector<basic_types> ft;

//                       geom_wrp->get_format(fn, ft);

//                       switch (ft[ev.m_field_id]) {
//                       case basic_types::type_int: {
//                         geom_wrp->template set_xfield<int>(ev.m_field_id, ev.m_atom_id,
//                                                            std::get<int>(ev.m_before));
//                         break;
//                       }
//                       case basic_types::type_real: {
//                         geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
//                                                               std::get<double>(ev.m_before));
//                         break;
//                       }
//                       case basic_types::type_double: {
//                         geom_wrp->template set_xfield<double>(ev.m_field_id, ev.m_atom_id,
//                                                               std::get<double>(ev.m_before));
//                         break;
//                       }
//                       case basic_types::type_float: {
//                         geom_wrp->template set_xfield<float>(ev.m_field_id, ev.m_atom_id,
//                                                              std::get<float>(ev.m_before));
//                         break;
//                       }
//                       case basic_types::type_bool: {
//                         geom_wrp->template set_xfield<bool>(ev.m_field_id, ev.m_atom_id,
//                                                             std::get<bool>(ev.m_before));
//                         break;
//                       }
//                       case basic_types::type_string: {
//                         geom_wrp->template set_xfield<STRING_EX>(
//                             ev.m_field_id, ev.m_atom_id,std::get<STRING_EX>(ev.m_before));
//                         break;
//                       }

                     },

                     [geom_wrp](select_atom_event_t &ev) {},
                     }, act);
    }

    commit_changes(true, true);
    p_cur_acts = p_tmp_acts;
    p_tmp_acts.clear();

    p_epoch_data[get_cur_epoch()] = p_cur_acts;
    p_cur_acts.clear();

  }

  std::string print_epoch_info(epoch_t epoch) {

    std::string message;

    if (!has_epoch(epoch)) {
      message = fmt::format("Epoch {} doesn't exsist!\n", epoch);
      return message;
    }

    auto epd_it = p_epoch_data.find(epoch);
    if (epd_it == end(p_epoch_data)) {
      message = fmt::format("Epoch {} is empty!\n", epoch);
      return message;
    }

    xgeometry<REAL, CELL> *geom_wrp = p_xgeom;


    auto &epd = epd_it->second;
    for (auto &event_inst : epd)
      std::visit(overloaded {

                     [&message, geom_wrp](auto arg) {},

                     [&message, geom_wrp](insert_atom_event_t<REAL> &ev) {
                       message += fmt::format("insert_atom_event id={}, name={}, pos={}\n",
                                              ev.m_atom_idx.value_or(geom_wrp->nat()),
                                              ev.m_atom_name, ev.m_atom_pos);
                     },

                     [&message, geom_wrp](change_atom_event_t<REAL> &ev) {
                       message += fmt::format("change_atom id={}, name from {} to {},"
                                              " pos from {} to {}\n",
                                              ev.m_atom_idx, ev.m_before_aname, ev.m_after_aname,
                                              ev.m_before_apos, ev.m_after_apos);
                     },

                     [&message, geom_wrp](change_atom_pos_event_t<REAL> &ev) {
                       //geom_wrp->change_pos(ev.m_atom_idx, ev.m_before_apos);
                       //ev.m_after_apos = geom_wrp->pos(ev.m_atom_idx);
                       message += fmt::format("change_atom_pos id={} from {} to {}\n",
                                              ev.m_atom_idx, ev.m_before_apos, ev.m_after_apos);
                     },

                     [&message, geom_wrp](erase_atom_event_t<REAL> &ev) {
                       //geom_wrp->insert(ev.m_atom_idx, ev.m_atom_name, ev.m_atom_pos);
                     },

                     [&message, geom_wrp](reorder_atoms_event_t &ev) {
                       //geom_wrp->reorder(ev.m_atoms_order);
                     },

                     [&message, geom_wrp](change_dim_event_t &ev) {
                       //geom_wrp->set_DIM(ev.old_dim);
                     },

                     [&message, geom_wrp](change_cell_event_t<REAL> &ev) {

                     },

                     [&message, geom_wrp](xfield_changed_event_t &ev) {

                     },

                     [&message, geom_wrp](select_atom_event_t &ev) {
                       message += fmt::format("select_atom_event id={} idx={} state={}\n",
                                              ev.m_atom.m_atm, ev.m_atom.m_idx, ev.m_state);
                     },
                     }, event_inst);

    return message;

  }

};

}

}

#endif
