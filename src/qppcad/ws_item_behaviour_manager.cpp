#include <qppcad/ws_item_behaviour_manager.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_comp_chem_data/ws_comp_chem_data.hpp>
#include <qppcad/ws_volume_data/ws_volume_data.hpp>
#include <QFileInfo>

using namespace qpp;
using namespace qpp::cad;

ws_item_behaviour_manager_t::ws_item_behaviour_manager_t() {

}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::load_ws_itm_from_file(
    const std::string &file_name,
    size_t io_bhv_idx,
    workspace_t *ws) {

  app_state_t* astate = app_state_t::get_inst();

  std::setlocale(LC_ALL, "C");

  astate->log(fmt::format("Loading ws_item from file {}", file_name));

  auto new_ws_item = fabric_by_type(m_ws_item_io[io_bhv_idx]->m_accepted_type);
  ws->add_item_to_ws(new_ws_item);

  if (new_ws_item) {
      std::ifstream input(file_name);
      new_ws_item->m_name = extract_base_name(file_name);
      m_ws_item_io[io_bhv_idx]->load_from_stream(input, new_ws_item.get(), ws);
      return new_ws_item;
    } else {
      return nullptr;
    }

  return nullptr;

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

void ws_item_behaviour_manager_t::save_ws_itm_to_file(std::string &file_name,
                                                      std::shared_ptr<ws_item_t> ws_item,
                                                      size_t bhv_id) {

  app_state_t* astate = app_state_t::get_inst();

  if (!ws_item) return;
  if (bhv_id < m_ws_item_io.size() && m_ws_item_io[bhv_id]->can_save() &&
      m_ws_item_io[bhv_id]->m_accepted_type == ws_item->get_type()) {
      astate->log(fmt::format("Saving ws_item[{}] to file {} from workspace {}",
                              ws_item->m_name, file_name, ws_item->m_parent_ws->m_ws_name));

      std::ofstream output(file_name);
      m_ws_item_io[bhv_id]->save_to_stream(output, ws_item.get());
    }

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

  size_t _ff_hash = astate->hash_reg->calc_hash(_full_name);

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

  astate->log(fmt::format("Registering file format {}[{}] - hash {}, ghash {}",
                          _full_name, _short_name, _ff_hash, _file_format_group_hash));

  return _ff_hash;
}

size_t ws_item_behaviour_manager_t::reg_ffg(std::string _full_name,
                                            std::string _short_name) {
  app_state_t *astate = app_state_t::get_inst();

  size_t _file_format_group_hash = astate->hash_reg->calc_hash(_full_name);

  auto it = m_file_format_groups.find(_file_format_group_hash);

  if (it == m_file_format_groups.end()) {
      ws_item_io_file_format_group_t new_file_format_group;
      new_file_format_group.m_full_name = _full_name;
      new_file_format_group.m_short_name = _short_name;
      m_file_format_groups.emplace(_file_format_group_hash, std::move(new_file_format_group));
    }

  astate->log(fmt::format("Registering file format group {}[{}] - hash {}",
                          _full_name, _short_name, _file_format_group_hash));

  return _file_format_group_hash;
}

size_t ws_item_behaviour_manager_t::reg_tool_grp(std::string _full_name) {

  app_state_t *astate = app_state_t::get_inst();

  ws_item_tool_group_t tool_grp;
  tool_grp.m_full_name = _full_name;
  size_t _g_hash = astate->hash_reg->calc_hash(_full_name);
  m_tools_groups.emplace(_g_hash, std::move(tool_grp));

  return _g_hash;

}

std::optional<size_t> ws_item_behaviour_manager_t::get_ff_by_finger_print(
    const std::string &file_name) {

  for (auto &elem : m_file_formats)
    for (auto &ffp : elem.second.m_finger_prints)
      if (file_name.find(ffp) != std::string::npos) {
          app_state_t *astate = app_state_t::get_inst();
          astate->log(fmt::format("Compare ff {} {} - fname {}",
                                  elem.first, elem.second.m_full_name, file_name));
          return std::optional<size_t>(elem.first);
        }

  return std::nullopt;

}

std::optional<size_t> ws_item_behaviour_manager_t::get_ff_by_short_name(
    const std::string &ffmt_short_name) {
  for (auto &elem : m_file_formats)
    if (elem.second.m_shortname == ffmt_short_name) return std::optional<size_t>(elem.first);
  return std::nullopt;
}

std::optional<size_t> ws_item_behaviour_manager_t::get_io_bhv_by_file_format(size_t file_format) {

  for (size_t i = 0; i < m_ws_item_io.size(); i++)
    if (m_ws_item_io[i]->m_accepted_file_format == file_format) {
        app_state_t *astate = app_state_t::get_inst();
        astate->log(fmt::format("Compare ff {} - io_bhv {}",file_format, i));
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

  astate->log(
        fmt::format("Registering io behaviour for type {}, file format[{}], save[{}], load[{}]",
                    accepted_type,
                    get_ff_full_name(accepted_file_format),
                    io_bhv_inst->can_save(),
                    io_bhv_inst->can_load()));

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

  astate->log(fmt::format("get_obj_insp_widget_sp with type_id = {}, query?", hash));

  auto it = m_obj_insp_widgets.find(hash);
  if (it != m_obj_insp_widgets.end() && it->second != nullptr) {
      astate->log(fmt::format("get_obj_insp_widget_sp with type_id = {}, exists", hash));
      return it->second;
    }

  if (it == m_obj_insp_widgets.end()) {
      auto it_f = m_obj_insp_fabric.find(hash);
      if (it_f != m_obj_insp_fabric.end()) {
          astate->log(fmt::format("get_obj_insp_widget_sp with type_id = {}, constructing", hash));
          auto cnstr = it_f->second();
          m_obj_insp_widgets.emplace(hash, cnstr);
          return it_f->second();
        }
      else return nullptr;
    }

  astate->log(fmt::format("get_obj_insp_widget_sp with type_id = {}, not found", hash));
  return nullptr;
}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::fabric_by_type(size_t type_id) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log(fmt::format("Fabric new ws_item with type_id = {}", type_id));

  auto it = m_fabric_ws_item.find(type_id);
  if (it != m_fabric_ws_item.end()) return it->second();

  astate->log(fmt::format("Cannot fabric new ws_item with type_id = {}!", type_id));
  return nullptr;

}

ws_item_t *ws_item_behaviour_manager_t::ws_item_fbr_by_type_p(size_t type_id) {

  app_state_t *astate = app_state_t::get_inst();

  astate->log(fmt::format("Fabric new ws_item with type_id = {}", type_id));

  if (type_id == ws_atoms_list_t::get_type_static())
    return new ws_atoms_list_t();

  if (type_id == ws_comp_chem_data_t::get_type_static())
    return new ws_comp_chem_data_t();

  if (type_id == ws_volume_data_t::get_type_static())
    return new ws_volume_data_t();

  astate->log(fmt::format("Cannot fabric new ws_item with type_id = {}!", type_id));
  return nullptr;

}


bool ws_item_io_behaviour_t::is_type_accepted(size_t _type) {
  return m_accepted_type == _type;
}

