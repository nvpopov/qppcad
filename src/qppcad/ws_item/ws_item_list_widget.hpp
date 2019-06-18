#ifndef QPPCAD_WS_ITEM_LIST_WIDGET
#define QPPCAD_WS_ITEM_LIST_WIDGET

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>

#include <QWidget>
#include <QListWidget>

namespace qpp {

  namespace cad {

    class ws_item_list_widget_t : public QListWidget {

        Q_OBJECT

      public:

        ws_item_t *m_master_item{nullptr};
        std::vector<ws_item_t*> m_sub_items;
        size_t m_ws_item_class{0};

        ws_item_list_widget_t(QWidget *parent = nullptr);
        void rebuild_sub_gvs(std::function<bool(ws_item_t *master, ws_item_t *slave)> comparator);
        void select_all_clicked();

    };

  } // namespace cad

} // namespace qpp

#endif
