#include <qppcad/hotkey_manager.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

hotkey_entry_t::hotkey_entry_t(std::string hotkey_keys,
                               std::string pycommand,
                               QMainWindow *main_window ) {


  m_pycommand = pycommand;
  m_is_pycommand = true;

  m_shortcut = std::make_unique<QShortcut>(main_window);
  m_key_sequence = QKeySequence(QString::fromStdString(hotkey_keys));
  m_shortcut->setKey(m_key_sequence);
  m_shortcut->setContext(Qt::ShortcutContext::ApplicationShortcut);

  connect(m_shortcut.get(),
          &QShortcut::activated,
          this,
          &hotkey_entry_t::activated);

}

std::string hotkey_entry_t::get_key_sequence() {

  return m_key_sequence.toString().toStdString();

}

std::string hotkey_entry_t::py_print() {
  return fmt::format("hk[key_seq={}, is_python={}, pycommand={}]",
                     m_key_sequence.toString().toStdString(),
                     m_is_pycommand,
                     m_pycommand);
}

void hotkey_entry_t::activated() {

  app_state_t *astate = app_state_t::get_inst();
 // astate->tlog("hotkey_entry_t::activated()");

  if (m_is_pycommand) {
      //astate->tlog("hotkey_entry_t::activated(m_is_pycommand=true)");
      astate->py_mgr->execute(m_pycommand);
    }

}

void hotkey_manager_t::reg_hotkey(std::string hk_comb, std::string pycommand) {

  QKeySequence match_ks(QString::fromStdString(hk_comb));

  //find existed key sequence
  auto exs_hk = std::find_if(std::begin(m_hotkeys),
                             std::end(m_hotkeys),
                             [&match_ks](auto rec){ return rec->m_key_sequence == match_ks;});

  if (exs_hk != std::end(m_hotkeys)) {
          exs_hk->get()->m_pycommand = pycommand;
          return;
    }

  auto new_hk = std::make_shared<hotkey_entry_t>(hk_comb, pycommand, m_main_window);
  m_hotkeys.push_back(new_hk);

}

void hotkey_manager_t::unreg_hotkey_by_key_sequence(std::string hk_comb) {

  QKeySequence match_ks(QString::fromStdString(hk_comb));

  //find existed key sequence
  auto exs_hk = std::find_if(std::begin(m_hotkeys),
                             std::end(m_hotkeys),
                             [&match_ks](auto rec){ return rec->m_key_sequence == match_ks;});

  if (exs_hk != std::end(m_hotkeys)) m_hotkeys.erase(exs_hk);

}

void hotkey_manager_t::unreg_hotkey_by_index(size_t hotkey_index) {

  if (hotkey_index >= m_hotkeys.size()) return;

  auto hkb = m_hotkeys.begin();
  std::advance(hkb, hotkey_index);
  m_hotkeys.erase(hkb);

}

void hotkey_manager_t::bootstrap_basic_hotkeys() {

  if (!m_hotkeys.empty()) return;

}

void hotkey_manager_t::save_to_settings(QSettings &settings) {

  settings.beginWriteArray("hk_array");

  for (int q = 0; q < m_hotkeys.size(); q++) {

      settings.setArrayIndex(q);
      settings.setValue("key_seq", m_hotkeys[q]->m_key_sequence.toString());
      settings.setValue("is_pycommand", m_hotkeys[q]->m_is_pycommand);
      settings.setValue("pycommand", QString::fromStdString(m_hotkeys[q]->m_pycommand));

    }

  settings.endArray();

}

void hotkey_manager_t::load_from_settings(QSettings &settings) {

  int hk_size = settings.beginReadArray("hk_array");

  for (int q = 0; q < hk_size; q++) {

      settings.setArrayIndex(q);

      QString r_key_seq = settings.value("key_seq").toString();
      bool r_is_pycommand = settings.value("is_pycommand").toBool();
      QString r_pycommand = settings.value("pycommand").toString();

      if (r_is_pycommand) {

          hotkey_entry_restore_info_t hk_ri;
          hk_ri.m_key_seq = r_key_seq.toStdString();
          hk_ri.m_is_pycommand = r_is_pycommand;
          hk_ri.m_pycommand = r_pycommand.toStdString();

          m_restore_info.push_back(std::move(hk_ri));

        }

    }

  settings.endArray();

}

void hotkey_manager_t::bootstrap_from_restore_info() {

  for (auto &rec : m_restore_info)
    if (rec.m_is_pycommand) reg_hotkey(rec.m_key_seq, rec.m_pycommand);

}
