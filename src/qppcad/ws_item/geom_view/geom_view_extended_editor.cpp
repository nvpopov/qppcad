#include <qppcad/ws_item/geom_view/geom_view_extended_editor.hpp>
#include <QResizeEvent>

using namespace qpp;
using namespace qpp::cad;

geom_view_extended_editor_t::geom_view_extended_editor_t() {

  ext_editor_gv_label = new QLabel(tr("geom_view_extended_editor_t"));
  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,0);
  setLayout(main_lt);

  xgeom_tv = new QTableView(this);
  xgeom_tv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  xgeom_tv->verticalHeader()->hide();
  xgeom_tv->setShowGrid(false);
  xgeom_tv->horizontalHeader()->setStretchLastSection(true);

  main_lt->addWidget(xgeom_tv);
  main_lt->addStretch(1);

  xgeom_tmdl = new xgeom_table_model_t;
  xgeom_tv->setModel(xgeom_tmdl);

}

void geom_view_extended_editor_t::bind_to_item(ws_item_t *_binding_item) {

  ws_item_extended_editor_t::bind_to_item(_binding_item);

  if (!_binding_item) {
      m_binded_gv = nullptr;
      xgeom_tmdl->unbind();
      xgeom_tv->update();
      return;
    }

  auto gv = _binding_item->cast_as<geom_view_t>();
  if (!gv) {
      m_binded_gv = nullptr;
      xgeom_tmdl->unbind();
      xgeom_tv->update();
      return;
    }

  m_binded_gv = gv;
  xgeom_tmdl->bind(m_binded_gv->m_geom.get());
  xgeom_tv->update();

}

void geom_view_extended_editor_t::update_from_ws_item() {

  ws_item_extended_editor_t::update_from_ws_item();

}

void geom_view_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  xgeom_tmdl->unbind();

}

void geom_view_extended_editor_t::resizeEvent(QResizeEvent *event) {

  if (xgeom_tv) {
      xgeom_tv->setFixedHeight(event->size().height() * 0.98);
    }

  ws_item_extended_editor_t::resizeEvent(event);

}
