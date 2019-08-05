#ifndef QPPCAD_HOTKEY_MANAGER
#define QPPCAD_HOTKEY_MANAGER

#include <qppcad/qppcad.hpp>
#include <QShortcut>

namespace qpp {

    namespace cad {

        class hotkey_entry_t : public QObject {

           Q_OBJECT

           public:

                QKeySequence m_key_sequence;
                std::unique_ptr<QShortcut> m_shortcut;
                hotkey_entry_t(std::string hotkey_keys);

                std::string get_key_sequence();

           public slots:

                void activated();

        };

        class hotkey_manager_t {

          public:

                std::vector<hotkey_entry_t> m_hotkeys;

                void reg_hotkey(std::string hk_comb);

        };

    } // namespace qpp::cad

} // namespace qpp

#endif
