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
  astate->tlog("hotkey_entry_t::activated()");

  if (m_is_pycommand) {
      astate->tlog("hotkey_entry_t::activated(m_is_pycommand=true)");
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

void hotkey_manager_t::unreg_hotkey(std::string hk_comb) {

  QKeySequence match_ks(QString::fromStdString(hk_comb));

  //find existed key sequence
  auto exs_hk = std::find_if(std::begin(m_hotkeys),
                             std::end(m_hotkeys),
                             [&match_ks](auto rec){ return rec->m_key_sequence == match_ks;});

  if (exs_hk != std::end(m_hotkeys)) m_hotkeys.erase(exs_hk);

}
