#include <qppcad/ws_item_behaviour_manager.hpp>
#include <qppcad/app_state.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <qppcad/ws_comp_chem_data/ws_comp_chem_data.hpp>
#include <qppcad/ws_volume_data/ws_volume_data.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_behaviour_manager_t::ws_item_behaviour_manager_t() {

}



void ws_item_behaviour_manager_t::load_ws_item_from_file(std::string &file_name,
                                                         ws_item_t *ws_item,
                                                         size_t file_format,
                                                         bool trust) {

  if (!ws_item) return;

  app_state_t* astate = app_state_t::get_inst();

  std::setlocale(LC_ALL, "C");

  astate->log(fmt::format("Loading ws_item from file {} to  workspace {}",
                          file_name, ws_item->m_parent_ws->m_ws_name));

  std::ifstream input(file_name);
  //m_ws_item_io[file_format].
  //load_ws_item_from_stream(input, ws_item, file_format);
}

void ws_item_behaviour_manager_t::load_ws_item_from_file(std::string &file_name,
                                                         ws_item_t *ws_item) {
  //if (!ws_item) return;
  for (size_t i = 0; i < m_ws_item_io.size(); i++) {

      if (!ws_item) {

          //create new ws_item
          if (m_ws_item_io[i]->can_load() && m_ws_item_io[i]->deduce_from_file_name(file_name)) {
              std::shared_ptr<ws_item_t> new_item{nullptr};
              new_item = fabric_by_type(m_ws_item_io[i]->m_accepted_type);
              ws_item = new_item.get();
              if (new_item) {
                  load_ws_item_from_file(file_name, new_item.get(), i, true);
                  return;
                }

            }

        } else {
          load_ws_item_from_file(file_name, ws_item, i, true);
        }
    }

}

void ws_item_behaviour_manager_t::save_ws_item_to_file(std::string &file_name,
                                                       ws_item_t *ws_item,
                                                       size_t file_format,
                                                       bool trust) {
  app_state_t* astate = app_state_t::get_inst();

  astate->log(fmt::format("Saving ws_item[{}] to file {} from workspace {}",
                          ws_item->m_name, file_name, ws_item->m_parent_ws->m_ws_name));

  std::ofstream output(file_name);
  //save_ws_item_to_stream(output, ws_item, file_format);

}

void ws_item_behaviour_manager_t::save_ws_item_to_file(std::string &file_name, ws_item_t *ws_item) {

  if (!ws_item) return;
  for (size_t i = 0; i < m_ws_item_io.size(); i++)
    if (m_ws_item_io[i]->is_type_accepted(ws_item->get_type()) && m_ws_item_io[i]->can_save()) {
        save_ws_item_to_file(file_name, ws_item, i, true);
      }
}

std::string ws_item_behaviour_manager_t::get_file_format_full_name(size_t _file_format_hash) {
  auto it = m_file_formats.find(_file_format_hash);
  if (it != m_file_formats.end()) return it->second.m_full_name;
  else return "NOT_FOUND";
}

size_t ws_item_behaviour_manager_t::register_file_format(std::string _full_name,
                                                         std::string _short_name) {
  app_state_t *astate = app_state_t::get_inst();

  size_t _file_format_hash = astate->hash_reg->calc_hash(_full_name);
  auto it = m_file_formats.find(_file_format_hash);

  if (it == m_file_formats.end()) {
      ws_item_io_file_format_t new_file_format;
      new_file_format.m_full_name = _full_name;
      new_file_format.m_shortname = _short_name;
      m_file_formats.insert(
            std::pair<size_t, ws_item_io_file_format_t>(_file_format_hash,
                                                        std::move(new_file_format)));
    }

  astate->log(fmt::format("Registering file format {}[{}] - hash {}",
                          _full_name, _short_name, _file_format_hash));

  return _file_format_hash;
}

void ws_item_behaviour_manager_t::register_io_behaviour(
    std::shared_ptr<ws_item_io_behaviour_t> io_bhv_inst,
    size_t accepted_file_format,
    size_t accepted_type) {

  app_state_t *astate = app_state_t::get_inst();

  io_bhv_inst->m_accepted_type = accepted_type;
  io_bhv_inst->m_accepted_file_format = accepted_file_format;

  astate->log(fmt::format("Registering io behaviour for type {}, ff {}, save[{}], load[{}]",
                          accepted_type,
                          get_file_format_full_name(accepted_file_format),
                          io_bhv_inst->can_save(),
                          io_bhv_inst->can_load()));

  m_ws_item_io.push_back(io_bhv_inst);
}

void ws_item_behaviour_manager_t::unregister_file_format(size_t _file_format_hash) {

}

std::shared_ptr<ws_item_t> ws_item_behaviour_manager_t::fabric_by_type(size_t type_id) {

  if (type_id == ws_atoms_list_t::get_type_static())
    return std::make_shared<ws_atoms_list_t>();

  if (type_id == ws_comp_chem_data_t::get_type_static())
    return std::make_shared<ws_comp_chem_data_t>();

  if (type_id == ws_volume_data_t::get_type_static())
    return std::make_shared<ws_volume_data_t>();

  return nullptr;
}

bool ws_item_io_behaviour_t::deduce_from_file_name(std::string &file_name) {
  return false;
}

bool ws_item_io_behaviour_t::is_type_accepted(size_t _type) {
  return m_accepted_type == _type;
}

