#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/core/app_state.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>
#include <qppcad/ws_item/volume_view/volume_view.hpp>

#include <QFileInfo>
#include <QDirIterator>

using namespace qpp;
using namespace qpp::cad;

ws_item_behaviour_manager_t::ws_item_behaviour_manager_t() {

}

void ws_item_behaviour_manager_t::load_fixtures_from_path(
    const std::vector<std::string> &file_paths) {

  app_state_t* astate = app_state_t::get_inst();

  for (const auto &rec : file_paths) {

      QDirIterator fxts_dir(QString::fromStdString(rec), QDirIterator::NoIteratorFlags);

      while (fxts_dir.hasNext()) {

          auto fxt_dir = fxts_dir.next();
          if (fxt_dir.endsWith("/..") || fxt_dir.endsWith("/.")) continue;

          auto fxt_manifest_fn = QString("%1/%2").arg(fxt_dir).arg("manifest.json");
          QFileInfo check_file(fxt_manifest_fn);

          astate->tlog("Loading fixture from path {}, manifest path = {}",
                       fxt_dir.toStdString(), fxt_manifest_fn.toStdString());

          if (check_file.exists() && check_file.isFile()) {

              fixture_info_t new_fxt;
              new_fxt.load_from_file(fxt_manifest_fn.toStdString(), fxt_dir.toStdString());

              if (new_fxt.m_initialized) {

                  size_t fixture_hash = astate->hash_reg->calc_hash(new_fxt.m_fxt_name);
                  astate->tlog("Fixture \"{}\" loaded!", new_fxt.m_fxt_name);
                  m_fixtures_info.insert({fixture_hash, std::move(new_fxt)});

                } else {
                  astate->tlog("Cannot load fixture!");
                }

            }

        } // end while

    }

}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::load_ws_itm_from_file(
    const std::string &file_name,
    size_t io_bhv_idx,
    workspace_t *ws) {

  app_state_t* astate = app_state_t::get_inst();

  std::setlocale(LC_ALL, "C");

  astate->tlog("Loading ws_item from file {}", file_name);

  auto new_ws_item = fbr_ws_item_by_type(m_ws_item_io[io_bhv_idx]->m_accepted_type);
  new_ws_item->m_genesis_file_name = file_name;
  ws->add_item_to_ws(new_ws_item);

  if (new_ws_item) {

      std::ifstream input(file_name);
      new_ws_item->m_name = extract_base_name(file_name);
      m_ws_item_io[io_bhv_idx]->load_from_stream(input, new_ws_item.get(), ws);
      return new_ws_item;

    } else {
      return nullptr;
    }

}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::load_ws_itm_from_file(
    const std::string &file_name,
    workspace_t *ws) {

  QFileInfo check_file(QString::fromStdString(file_name));

  if (!check_file.exists() || !check_file.isFile()) return nullptr;

  auto file_format = get_ff_by_finger_print(file_name);

  if (file_format) {

      std::optional<size_t> io_bhv_id = get_io_bhv_by_file_format(*file_format);

      if (io_bhv_id) {
          auto ret_sp = load_ws_itm_from_file(file_name, *io_bhv_id, ws);
          return ret_sp;
        }

    }

  return nullptr;

}

bool ws_item_behaviour_manager_t::save_ws_itm_to_file(std::string &file_name,
                                                      std::shared_ptr<ws_item_t> ws_item,
                                                      size_t bhv_id,
                                                      std::string &message) {

  app_state_t* astate = app_state_t::get_inst();

  if (!ws_item) return false;

  if (bhv_id < m_ws_item_io.size() && m_ws_item_io[bhv_id]->can_save() &&
      m_ws_item_io[bhv_id]->m_accepted_type == ws_item->get_type()) {
      astate->tlog("Saving ws_item[{}] to file {} from workspace {}",
                   ws_item->m_name, file_name, ws_item->m_parent_ws->m_ws_name);

      bool check = m_ws_item_io[bhv_id]->check_before_save(ws_item.get(), message);
      if (check) {

          std::ofstream output(file_name);
          m_ws_item_io[bhv_id]->save_to_stream(output, ws_item.get());
          return true;

        } else {

          astate->tlog("Checking failed for ws_item={}, file={}, workspace={}",
                       ws_item->m_name, file_name, ws_item->m_parent_ws->m_ws_name);
          return false;

        }

    }

  return false;

}

std::string ws_item_behaviour_manager_t::get_ff_full_name(size_t _file_format_hash) {

  auto it = m_file_formats.find(_file_format_hash);
  if (it != m_file_formats.end()) return it->second.m_shortname;
  else return "NOT_FOUND";

}

std::string ws_item_behaviour_manager_t::get_ff_short_name(size_t _file_format_hash) {

  auto it = m_file_formats.find(_file_format_hash);
  if (it != m_file_formats.end()) return it->second.m_shortname;
  else return "NOT_FOUND";

}

size_t ws_item_behaviour_manager_t::reg_ff(std::string _full_name,
                                           std::string _short_name,
                                           size_t _file_format_group_hash,
                                           std::vector<std::string> _finger_prints) {

  app_state_t *astate = app_state_t::get_inst();

  size_t _ff_hash = astate->hash_reg->calc_hash_ub(_full_name);

  auto it = m_file_formats.find(_ff_hash);

  if (it == m_file_formats.end()) {

      ws_item_io_file_format_t new_file_format;
      new_file_format.m_full_name = _full_name;
      new_file_format.m_shortname = _short_name;
      new_file_format.m_finger_prints = _finger_prints;
      new_file_format.m_group_hash = _file_format_group_hash;

      auto it_ffg = m_file_format_groups.find(_file_format_group_hash);
      if (it_ffg != m_file_format_groups.end())
        m_file_format_groups[_file_format_group_hash].m_ffs_lookup.insert(_ff_hash);

      m_file_formats.emplace(_ff_hash, std::move(new_file_format));

    }

  astate->tlog("Registering file format {}[{}] - hash {}, ghash {}",
               _full_name, _short_name, _ff_hash, _file_format_group_hash);

  return _ff_hash;

}

size_t ws_item_behaviour_manager_t::reg_ffg(std::string _full_name, std::string _short_name) {

  app_state_t *astate = app_state_t::get_inst();

  size_t _file_format_group_hash = astate->hash_reg->calc_hash_ub(_full_name);

  auto it = m_file_format_groups.find(_file_format_group_hash);

  if (it == m_file_format_groups.end()) {

      ws_item_io_file_format_group_t new_file_format_group;
      new_file_format_group.m_full_name = _full_name;
      new_file_format_group.m_short_name = _short_name;
      m_file_format_groups.emplace(_file_format_group_hash, std::move(new_file_format_group));

    }

  astate->tlog("Registering file format group {}[{}] - hash {}",
               _full_name, _short_name, _file_format_group_hash);

  return _file_format_group_hash;

}

size_t ws_item_behaviour_manager_t::reg_tool_grp(std::string _full_name) {

  app_state_t *astate = app_state_t::get_inst();

  ws_item_tool_group_t tool_grp;
  tool_grp.m_full_name = _full_name;
  size_t _g_hash = astate->hash_reg->calc_hash_ub(_full_name);
  m_tools_groups.emplace(_g_hash, std::move(tool_grp));

  return _g_hash;

}

size_t ws_item_behaviour_manager_t::reg_tool(
    std::string _full_name,
    size_t _g_hash,
    size_t _t_hash,
    bool _itm_req, std::function<std::shared_ptr<ws_item_tool_t> ()> _fabric) {

  app_state_t *astate = app_state_t::get_inst();

  ws_item_tool_info_t tinfo;
  tinfo.m_full_name = _full_name;
  tinfo.m_accepted_type = _t_hash;
  tinfo.m_group_hash = _g_hash;
  tinfo.m_item_required = _itm_req;
  tinfo.m_fabric = _fabric;

  size_t tinfo_hash = astate->hash_reg->calc_hash_ub(_full_name);

  m_tools_info.emplace(tinfo_hash, std::move(tinfo));
  return tinfo_hash;

}

size_t ws_item_behaviour_manager_t::reg_sflow_grp(std::string group_name) {

  app_state_t *astate = app_state_t::get_inst();

  size_t _g_hash = astate->hash_reg->calc_hash_ub(group_name);

  sflow_node_group_info_t sflow_grp;
  sflow_grp.m_group_name = group_name;
  sflow_grp.m_group_hash = _g_hash;

  m_sflow_node_group_info.emplace(_g_hash, std::move(sflow_grp));

  return _g_hash;

}

size_t ws_item_behaviour_manager_t::reg_reg_sf_fbr(
    std::string _full_name,
    size_t _g_hash,
    std::function<std::shared_ptr<sflow_node_t> ()> _fabric) {

  app_state_t *astate = app_state_t::get_inst();

  sflow_node_info_t sinfo;
  sinfo.m_full_name = _full_name;
  sinfo.m_group_hash = _g_hash;
  sinfo.m_fabric = _fabric;

  size_t sinfo_hash = astate->hash_reg->calc_hash_ub(_full_name);

  m_sflow_node_info.emplace(sinfo_hash, std::move(sinfo));
  return sinfo_hash;

}

void ws_item_behaviour_manager_t::exec_tool(ws_item_t* item,
                                            size_t tool_hash,
                                            uint32_t _error_ctx) {

  auto tinfo = m_tools_info.find(tool_hash);

  //is tool info exists?
  if (tinfo == m_tools_info.end()) return;

  //item must exsist if tool needs it
  if (tinfo->second.m_item_required && !item) return;

  //item type must be equal to accepted type
  if (tinfo->second.m_item_required &&
      item->get_type() != tinfo->second.m_accepted_type) return;

  auto tool_inst = tinfo->second.m_fabric();

  //bypass item
  tool_inst->exec(item, _error_ctx);

}

ws_item_tool_type_e ws_item_behaviour_manager_t::get_tool_type(size_t tool_hash) {

  auto tinfo = m_tools_info.find(tool_hash);
  if (tinfo == m_tools_info.end()) return ws_item_tool_type_e::ws_item_tool_invalid;
  return tinfo->second.m_tool_type;

}

void ws_item_behaviour_manager_t::exec_tool_by_name(std::string tool_name,
                                                    ws_item_t *item,
                                                    uint32_t _error_ctx) {

  auto it = std::find_if(m_tools_info.begin(),
                         m_tools_info.end(),
                         [&tool_name](const auto &e){return e.second.m_full_name == tool_name;});

  if (it != m_tools_info.end()) exec_tool(item, it->first, _error_ctx);

}

std::optional<size_t> ws_item_behaviour_manager_t::get_ff_by_finger_print(
    const std::string &file_name) {

  for (auto &elem : m_file_formats)
    for (auto &ffp : elem.second.m_finger_prints)
      if (file_name.find(ffp) != std::string::npos) {

          app_state_t *astate = app_state_t::get_inst();
          astate->tlog("Compare ff {} {} - fname {}",
                       elem.first, elem.second.m_full_name, file_name);
          return std::optional<size_t>(elem.first);

        }

  return std::nullopt;

}

std::optional<size_t> ws_item_behaviour_manager_t::get_ff_by_short_name(
    const std::string &ffmt_short_name) {

  auto find_iter = std::find_if(std::begin(m_file_formats), std::end(m_file_formats),
                                [&ffmt_short_name](const auto &rec){
                                return rec.second.m_shortname == ffmt_short_name;});

  if (find_iter != m_file_formats.cend()) return std::optional<size_t>(find_iter->first);

  return std::nullopt;

}

std::optional<size_t> ws_item_behaviour_manager_t::get_io_bhv_by_file_format(size_t file_format) {

  for (size_t i = 0; i < m_ws_item_io.size(); i++)
    if (m_ws_item_io[i]->m_accepted_file_format == file_format) {

        app_state_t *astate = app_state_t::get_inst();
        astate->tlog("Compare ff {} - io_bhv {}",file_format, i);
        return std::optional<size_t>(i);

      }

  return std::nullopt;

}

std::optional<size_t> ws_item_behaviour_manager_t::get_io_bhv_by_file_format_ex(size_t file_format,
                                                                                size_t type_hash) {
  for (size_t i = 0; i < m_ws_item_io.size(); i++)
    if (m_ws_item_io[i]->m_accepted_file_format == file_format &&
        m_ws_item_io[i]->m_accepted_type == type_hash)
      return std::optional<size_t>(i);

  return std::nullopt;

}

void ws_item_behaviour_manager_t::reg_io_bhv(
    std::shared_ptr<ws_item_io_behaviour_t> io_bhv_inst,
    size_t accepted_file_format,
    size_t accepted_type) {

  app_state_t *astate = app_state_t::get_inst();

  io_bhv_inst->m_accepted_type = accepted_type;
  io_bhv_inst->m_accepted_file_format = accepted_file_format;

  astate->tlog("Registering io behaviour for type {}, file format[{}], save[{}], load[{}]",
               accepted_type,
               get_ff_full_name(accepted_file_format),
               io_bhv_inst->can_save(),
               io_bhv_inst->can_load());

  m_ws_item_io.push_back(io_bhv_inst);

}

void ws_item_behaviour_manager_t::unreg_ff(size_t _file_format_hash) {
  auto it = m_file_formats.find(_file_format_hash);
  if (it != m_file_formats.end()) m_file_formats.erase(it);
}

void ws_item_behaviour_manager_t::reg_item_fbr(size_t hash,
                                               std::function<std::shared_ptr<ws_item_t>()> func) {
  m_fabric_ws_item[hash] = func;
}

void ws_item_behaviour_manager_t::reg_obj_insp_fbr(
    size_t hash,
    std::function<std::shared_ptr<ws_item_obj_insp_widget_t> ()> func) {

  m_obj_insp_fabric[hash] = func;

}

void ws_item_behaviour_manager_t::reg_ext_editor_fbr(
    size_t hash,
    size_t editor_order,
    std::string editor_name,
    std::function<std::shared_ptr<ws_item_extended_editor_t>()> func) {

  ws_item_extended_editor_info_t new_ee_info;
  new_ee_info.m_type = hash;
  new_ee_info.m_order = editor_order;
  new_ee_info.m_full_name = editor_name;
  new_ee_info.m_fabric = func;

  m_ext_editors_info[{hash, editor_order}] = std::move(new_ee_info);

}

void ws_item_behaviour_manager_t::reg_toolbar_elem_fbr(
    size_t hash,
    std::string editor_name,
    std::function<std::shared_ptr<toolbar_element_t> ()> func) {

}

bool ws_item_behaviour_manager_t::is_obj_insp_fbr_exists(size_t hash) {

  auto it = m_obj_insp_fabric.find(hash);
  return it != m_obj_insp_fabric.end();

}

bool ws_item_behaviour_manager_t::is_obj_insp_exists(size_t hash) {

  auto it = m_obj_insp_widgets.find(hash);
  return it != m_obj_insp_widgets.end();

}

std::shared_ptr<ws_item_obj_insp_widget_t> ws_item_behaviour_manager_t::get_obj_insp_widget_sp(
    size_t hash) {

  app_state_t *astate = app_state_t::get_inst();

  astate->tlog("get_obj_insp_widget_sp with type_id = {}, query?", hash);

  auto it = m_obj_insp_widgets.find(hash);
  if (it != m_obj_insp_widgets.end() && it->second != nullptr) {
      astate->tlog("get_obj_insp_widget_sp with type_id = {}, exists", hash);
      return it->second;
    }

  if (it == m_obj_insp_widgets.end()) {
      auto it_f = m_obj_insp_fabric.find(hash);
      if (it_f != m_obj_insp_fabric.end()) {

          astate->tlog("get_obj_insp_widget_sp with type_id = {}, constructing", hash);
          auto cnstr = it_f->second();
          m_obj_insp_widgets.emplace(hash, cnstr);
          return cnstr;

        }
      else {
          return nullptr;
        }
    }

  astate->tlog("get_obj_insp_widget_sp with type_id = {}, not found", hash);

  return nullptr;

}

void ws_item_behaviour_manager_t::cache_obj_insp_widgets() {

  auto obj_insp_w = get_obj_insp_widget_sp(geom_view_t::get_type_static());

}

std::shared_ptr<ws_item_extended_editor_t> ws_item_behaviour_manager_t::get_ext_editor_widget_sp(
    size_t hash, size_t ed_order) {

  app_state_t *astate = app_state_t::get_inst();

  astate->tlog("get_ext_editor_widget_sp with type_id = {}, query?", hash);

  auto it = m_ext_editors_info.find({hash, ed_order});
  if (it != m_ext_editors_info.end()) {

      if (it->second.m_inst) {
          astate->tlog("get_ext_editor_widget_sp with type_id = {}, exists", hash);
        } else {
          astate->tlog("get_ext_editor_widget_sp with type_id = {}, constr.", hash);
           it->second.m_inst = it->second.m_fabric();
        }
      return it->second.m_inst;
    }

  astate->tlog("get_ext_editor_widget_sp with type_id = {}, not found", hash);

  return nullptr;

}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::fbr_ws_item_by_type(size_t type_id) {

  app_state_t *astate = app_state_t::get_inst();

  astate->tlog("Fabric new ws_item with type_id = {}", type_id);

  auto it = m_fabric_ws_item.find(type_id);
  if (it != m_fabric_ws_item.end()) return it->second();

  astate->tlog("Cannot fabric new ws_item with type_id = {}!", type_id);
  return nullptr;

}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::fbr_ws_item_by_name(
    const std::string &type_name) {

  app_state_t *astate = app_state_t::get_inst();
  return fbr_ws_item_by_type(astate->hash_reg->calc_hash_ub<std::string>(type_name));

}

bool ws_item_io_behaviour_t::is_type_accepted(size_t _type) {
  return m_accepted_type == _type;
}

