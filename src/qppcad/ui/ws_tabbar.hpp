#ifndef QPPCAD_WS_TABBAR
#define QPPCAD_WS_TABBAR

#include <qppcad/core/qppcad.hpp>
#include <QTabBar>

namespace qpp {

  namespace cad {

    class ws_tabbar_t : public QTabBar {

        Q_OBJECT

      public:

        explicit ws_tabbar_t(QWidget *parent = nullptr);
        void update_tabs();

      public slots:

        void tabs_closed(int index);
        void cur_ws_changed();
        void wss_changed_slot();
        void current_changed(int current);
        void tab_double_clicked(int index);

    };

  } // namespace cad

} // namespace qpp

#endif
