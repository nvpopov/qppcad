#ifndef QPPCAD_HOTKEY_MANAGER
#define QPPCAD_HOTKEY_MANAGER

#include <qppcad/qppcad.hpp>
#include <QShortcut>
#include <QMainWindow>
#include <QSettings>

namespace qpp {

  namespace cad {

    struct hotkey_entry_restore_info_t {

        std::string m_key_seq;
        bool m_is_pycommand;
        std::string m_pycommand;

    };

    class hotkey_entry_t : public QObject {

        Q_OBJECT

      public:

        QKeySequence m_key_sequence;
        std::unique_ptr<QShortcut> m_shortcut;
        std::string m_pycommand;
        bool m_is_pycommand{false};
        hotkey_entry_t(std::string hotkey_keys,
                       std::string pycommand,
                       QMainWindow *main_window = nullptr);

        std::string get_key_sequence();
        std::string py_print();

      public slots:

        void activated();

    };

    class hotkey_manager_t {

      public:

        std::vector<std::shared_ptr<hotkey_entry_t> > m_hotkeys;
        std::vector<hotkey_entry_restore_info_t> m_restore_info;

        QMainWindow *m_main_window{nullptr};
        void reg_hotkey(std::string hk_comb, std::string pycommand);
        void unreg_hotkey_by_key_sequence(std::string hk_comb);
        void unreg_hotkey_by_index(size_t hotkey_index);
        void bootstrap_basic_hotkeys();
        void save_to_settings(QSettings &settings);
        void load_from_settings(QSettings &settings);
        void bootstrap_from_restore_info();

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
