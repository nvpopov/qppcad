#include <qppcad/tools/structure_similarity/structure_similarity.hpp>
#include <qppcad/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

void structure_similarity_tool_t::exec(ws_item_t *item) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      structure_similarity_widget_t scw;

      int ret_code = scw.exec();

    }

}

structure_similarity_widget_t::structure_similarity_widget_t() : QDialog () {

  setWindowTitle(tr("Structure similarity"));
  widget_layout = new QVBoxLayout;
  setLayout(widget_layout);

  gb_select_actors = new QGroupBox(tr("Actors"));
  gb_select_actors_lt = new QFormLayout;

  cmb_it1 = new QComboBox;
  cmb_ws1 = new QComboBox;
  cmb_it2 = new QComboBox;
  cmb_ws2 = new QComboBox;

  gb_select_actors_lt->addRow(tr("Workspace №1"), cmb_ws1);
  gb_select_actors_lt->addRow(tr("Item №1"), cmb_it1);
  gb_select_actors_lt->addRow(tr("Workspace №2"), cmb_ws2);
  gb_select_actors_lt->addRow(tr("Item №2"), cmb_it2);

  gb_select_actors->setLayout(gb_select_actors_lt);

  widget_layout->addWidget(gb_select_actors);

  connect(cmb_ws1,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_ws1_changed);

  connect(cmb_ws2,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this,
          &structure_similarity_widget_t::cmb_ws2_changed);

  cmb_ws(cmb_ws1);
  cmb_ws(cmb_ws2);

}

void structure_similarity_widget_t::cmb_ws(QComboBox *_cmb) {

  app_state_t *astate = app_state_t::get_inst();

  if (astate->ws_manager->has_wss()) {

      _cmb->clear();
      _cmb->setEnabled(true);
      for (size_t i = 0; i < astate->ws_manager->m_ws.size(); i++)
        _cmb->addItem(QString::fromStdString(astate->ws_manager->m_ws[i]->m_ws_name));

    } else {

      _cmb->clear();
      _cmb->setEnabled(false);

    }

}

void structure_similarity_widget_t::cmb_it(QComboBox *_cmb, std::shared_ptr<workspace_t> _ws) {

  app_state_t *astate = app_state_t::get_inst();

  if (!_ws->m_ws_items.empty()) {

      _cmb->clear();
      _cmb->setEnabled(true);
      for (size_t i = 0; i < _ws->m_ws_items.size(); i++)
        _cmb->addItem(tr("[%1]%2")
                      .arg(i)
                      .arg(QString::fromStdString(_ws->m_ws_items[i]->m_name)));

    } else {

      _cmb->clear();
      _cmb->setEnabled(false);

    }

}

void structure_similarity_widget_t::cmb_ws1_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();
  cmb_it(cmb_it1, astate->ws_manager->m_ws[cmb_ws1->currentIndex()]);

}

void structure_similarity_widget_t::cmb_ws2_changed(int index) {

  app_state_t *astate = app_state_t::get_inst();
  cmb_it(cmb_it2, astate->ws_manager->m_ws[cmb_ws2->currentIndex()]);

}
