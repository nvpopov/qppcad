#include <qppcad/hotkey_manager.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

hotkey_entry_t::hotkey_entry_t(std::string hotkey_keys) {

    m_shortcut = std::make_unique<QShortcut>(nullptr);
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

void hotkey_entry_t::activated() {

  app_state_t::get_inst()->tlog("hotkey_entry_t::activated()");

}

void hotkey_manager_t::reg_hotkey(std::string hk_comb) {

  hotkey_entry_t hk_new{hk_comb};

  //m_hotkeys.push_back(std::move(hk_new));

}
