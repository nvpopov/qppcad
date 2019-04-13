#include <qppcad/ws_item/pgf_producer/pgf_producer.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/psg_view/psg_view.hpp>
#include <qppcad/app_state.hpp>

#include <geom/builders.hpp>

using namespace qpp;
using namespace qpp::cad;

pgf_producer_t::pgf_producer_t() {

  set_default_flags(ws_item_flags_default);

}

void pgf_producer_t::vote_for_view_vectors(vector3<float> &out_look_pos,
                                          vector3<float> &out_look_at) {
  //do nothing
}

void pgf_producer_t::render() {
 //do nothing
}

bool pgf_producer_t::mouse_click(ray_t<float> *click_ray) {
  return false;
}

std::string pgf_producer_t::compose_type_descr() {
  return "pgf_producer";
}

void pgf_producer_t::update(float delta_time) {
  //do nothing
}

float pgf_producer_t::get_bb_prescaller() {
  return 1.0f;
}

bool pgf_producer_t::check_consistency() {

  app_state_t* astate = app_state_t::get_inst();

  astate->log(fmt::format("pgf_producer_t::check_consistency() -> "
                          "src.is_valid? = {}, "
                          "dst.is_valid? = {}, "
                          "psg.is_valid? = {}",
                          m_src_gv != nullptr,
                          m_dst_gv != nullptr,
                          m_sg_psg != nullptr));

  return m_src_gv && m_dst_gv && m_sg_psg /*&& m_src_gv->m_geom->DIM == m_dst_gv->m_geom->DIM*/;

}

void pgf_producer_t::compose_from_array_group() {

  app_state_t* astate = app_state_t::get_inst();

  m_imd.clear();
  m_imd.clear_type_table();
  if (m_sg_psg) m_imd.cell = gen_cell<float, qpp::matrix3<float> >(m_sg_psg->m_ag->group);
  m_imd.cell.auto_orders();

  //m_orders_range.clear();
  m_orders_range.resize(m_imd.cell.DIM);

  astate->tlog("pgf_producer_t::compose_from_array_group() ->");
  for (size_t i = 0; i < m_orders_range.size(); i++) {
      m_orders_range[i][0] = 0;
      m_orders_range[i][1] = 0;
      m_orders_range[i][2] = m_imd.cell.end()(i);
      astate->tlog(" m_orders_range[{}] = [0] = {}, [1] = {}, [2] = {}",
                   i,
                   m_orders_range[i][0],
                   m_orders_range[i][1],
                   m_orders_range[i][2]);
    }

}

uint32_t pgf_producer_t::get_amount_of_selected_content() {
  return 0;
}

size_t pgf_producer_t::get_content_count() {
  return 0;
}

void pgf_producer_t::save_to_json(json &data) {

  ws_item_t::save_to_json(data);
  save_ws_item_field(JSON_PGF_PRODUCER_SRC, m_src, data);
  save_ws_item_field(JSON_PGF_PRODUCER_DST, m_dst, data);
  save_ws_item_field(JSON_PGF_PRODUCER_PSG, m_psg, data);

}

void pgf_producer_t::load_from_json(json &data, repair_connection_info_t &rep_info) {

  ws_item_t::load_from_json(data, rep_info);
  load_ws_item_field(JSON_PGF_PRODUCER_SRC, &m_src, data, rep_info);
  load_ws_item_field(JSON_PGF_PRODUCER_DST, &m_dst, data, rep_info);
  load_ws_item_field(JSON_PGF_PRODUCER_PSG, &m_psg, data, rep_info);

}

bool pgf_producer_t::can_be_written_to_json() {
  return true;
}

void pgf_producer_t::generate_geom() {

  if (!check_consistency()) return;

  app_state_t* astate = app_state_t::get_inst();

  m_dst_gv->m_tws_tr->do_action(act_lock);
  m_dst_gv->m_tws_tr->do_action(act_lock_img);
  m_dst_gv->m_tws_tr->do_action(act_clear_all);

  m_dst_gv->m_geom->clear();
  m_dst_gv->m_geom->clear_type_table();

  //print out cell dimensions
  astate->tlog("pgf_producer_t::generate_geom() info:");
  astate->tlog("m_imd.cell.DIM = {}", m_imd.cell.DIM);
  astate->tlog("m_orders_range.size() = {}", m_orders_range.size());
  astate->tlog("m_imd.cell._begin = {}", m_imd.cell._begin);
  astate->tlog("m_imd.cell._end = {}", m_imd.cell._end);

  //generate geom
  if (m_orders_range.size() == m_imd.DIM) {

      astate->tlog("pgf_producer_t::generate_geom() entering in generation process");

      size_t DIM = m_imd.DIM;

      index gen_begin = index::D(DIM);
      index gen_end = index::D(DIM);

      for (size_t i = 0; i < DIM; i++) {
          gen_begin(i) = m_orders_range[i][0];
          gen_end(i) = m_orders_range[i][1];
        }

      astate->tlog("pgf_producer_t::generate_geom() copying atoms to intermediate");
      for (size_t i = 0; i < m_src_gv->m_geom->nat(); i++)
        m_imd.add(m_src_gv->m_geom->atom_name(i), m_src_gv->m_geom->pos(i));

      astate->tlog("pgf_producer_t::generate_geom() replicate");
      replicate(*(m_dst_gv->m_geom.get()),
                m_imd,
                m_imd.cell,
                m_imd.cell._begin,
                m_imd.cell._end);

      astate->tlog("pgf_producer_t::generate_geom() exiting in generation process");

    }

  m_dst_gv->m_tws_tr->do_action(act_rebuild_all);

  astate->make_viewport_dirty();

}

void pgf_producer_t::updated_internally(uint32_t update_reason) {

  app_state_t* astate = app_state_t::get_inst();
  astate->log("pgf_producer_t::updated_internally()");

  if (update_reason & ws_item_updf_generic) {

      auto clean_intermediates = [this]() {
          this->m_src_gv = nullptr;
          this->m_dst_gv = nullptr;
          this->m_sg_psg = nullptr;
        };

      //check src
      if (!m_src) {
          clean_intermediates();
          astate->log("pgf_producer_t::updated_internally() !m_src");
          return;
        }

      auto _src_as_gv = m_src->cast_as<geom_view_t>();
      if (!_src_as_gv) {
          astate->log("pgf_producer_t::updated_internally() !_src_as_gv");
          clean_intermediates();
          return;
        }

      m_src_gv = _src_as_gv;

      if (m_src == m_dst) {
          astate->log("pgf_producer_t::updated_internally() m_src == m_dst");
          clean_intermediates();
          return;
        }

      //check dst
      if (!m_dst) {
          astate->log("pgf_producer_t::updated_internally() !m_dst");
          clean_intermediates();
          return;
        }

      auto _dst_as_gv = m_dst->cast_as<geom_view_t>();
      if (!_dst_as_gv) {
          astate->log("pgf_producer_t::updated_internally() _dst_as_gv");
          clean_intermediates();
          return;
        }

      m_dst_gv = _dst_as_gv;

      //check psg
      if (!m_psg) {
          astate->log("pgf_producer_t::updated_internally() !m_ag");
          clean_intermediates();
          return;
        }

      auto _ag_as_psg = m_psg->cast_as<psg_view_t>();
      if (!_ag_as_psg) {
          astate->log("pgf_producer_t::updated_internally() !_ag_as_psg");
          clean_intermediates();
          return;
        }

      m_sg_psg = _ag_as_psg;
      compose_from_array_group();

      if (is_selected()) astate->astate_evd->cur_ws_selected_item_need_to_update_obj_insp();

    }

  generate_geom();

}

