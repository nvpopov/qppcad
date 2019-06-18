#include <qppcad/ws_item/ws_item_list_widget.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

ws_item_list_widget_t::ws_item_list_widget_t(QWidget *parent) {

}

void ws_item_list_widget_t::rebuild_sub_gvs(
    std::function<bool(ws_item_t *master, ws_item_t *slave)> comparator) {

  app_state_t *astate = app_state_t::get_inst();

  clear();

  if (!m_master_item) return;

  for (auto ws : astate->ws_mgr->m_ws)
    for (auto ws_item : ws->m_ws_items)
      if (ws_item && ws_item->get_type() == m_ws_item_class
          && comparator(m_master_item, ws_item.get())) {

            QListWidgetItem *list_item = new QListWidgetItem(this);

            std::string list_item_name = fmt::format("{}/{}",
                                                     ws_item->m_parent_ws->m_ws_name,
                                                     ws_item->m_name);

            list_item->setText(QString::fromStdString(list_item_name));
            list_item->setCheckState(Qt::Unchecked);

            m_sub_items.push_back(ws_item.get());

          }

}

void ws_item_list_widget_t::select_all_clicked() {

  for (size_t i = 0; i < count(); i++) {
      QListWidgetItem *item = this->item(i);
      item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }

}
