#include <qppcad/ws_item/geom_view/geom_view_extended_editor.hpp>
#include <qppcad/app_state.hpp>
#include <QResizeEvent>
#include <QInputDialog>

using namespace qpp;
using namespace qpp::cad;

geom_view_extended_editor_t::geom_view_extended_editor_t() {

  app_state_t *astate = app_state_t::get_inst();

  ext_editor_gv_label = new QLabel(tr("geom_view_extended_editor_t"));
  main_lt = new QVBoxLayout;
  main_lt->setContentsMargins(0,0,10,0);
  setLayout(main_lt);

  xgeom_tv = new QTableView(this);
  xgeom_tv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  xgeom_tv->setShowGrid(false);
  xgeom_tv->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
  xgeom_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

  xgeom_tv->setFocusPolicy(Qt::NoFocus);
  xgeom_tv->setSelectionMode(QAbstractItemView::NoSelection);

  main_lt->addWidget(xgeom_tv);
  main_lt->addStretch(1);

  xgeom_tmdl = new xgeom_table_model_t;
  xgeom_tv->setModel(xgeom_tmdl);

  connect(astate->astate_evd,
          &app_state_event_disp_t::cur_ws_selected_atoms_list_selection_changed_signal,
          this,
          &geom_view_extended_editor_t::selection_changed);

  connect(xgeom_tv->verticalHeader(),
          &QHeaderView::sectionDoubleClicked,
          this,
          &geom_view_extended_editor_t::header_vertical_double_clicked);

  connect(xgeom_tv->horizontalHeader(),
          &QHeaderView::sectionDoubleClicked,
          this,
          &geom_view_extended_editor_t::header_horizontal_clicked);

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
  xgeom_tmdl->bind(m_binded_gv);
  xgeom_tv->update();
  xgeom_tv->resizeColumnsToContents();

}

void geom_view_extended_editor_t::update_from_ws_item() {

  xgeom_tv->update();
  ws_item_extended_editor_t::update_from_ws_item();

}

void geom_view_extended_editor_t::unbind_item() {

  ws_item_extended_editor_t::unbind_item();
  xgeom_tmdl->unbind();

}

bool geom_view_extended_editor_t::can_be_binded_to(ws_item_t *item) {

  return true;

}

void geom_view_extended_editor_t::selection_changed() {

  if (m_binded_gv) {
      xgeom_tv->update();
    }

}

void geom_view_extended_editor_t::resizeEvent(QResizeEvent *event) {

  if (xgeom_tv) {
      xgeom_tv->setFixedHeight(event->size().height() * 0.98);
    }

  ws_item_extended_editor_t::resizeEvent(event);

}

void geom_view_extended_editor_t::header_vertical_double_clicked(int logical_index) {

  if (xgeom_tv && xgeom_tmdl && m_binded_gv) {

      //std::cout << "@int logical_index =" << logical_index << std::endl;
      m_binded_gv->toggle_atom_selection(logical_index);

      if (m_binded_gv->m_parent_ws &&
          m_binded_gv->m_parent_ws->m_edit_type != ws_edit_e::edit_content)
        m_binded_gv->m_parent_ws->set_edit_type(ws_edit_e::edit_content);

    }

}

void geom_view_extended_editor_t::header_horizontal_clicked(int logical_index) {

  if (!xgeom_tv || !xgeom_tmdl || !m_binded_gv) return;
  if (m_binded_gv->m_atom_idx_sel.empty()) return;

  app_state_t *astate = app_state_t::get_inst();

  basic_types field_type = m_binded_gv->m_geom->field_type(logical_index);

  switch (field_type) {
    case basic_types::type_bool : {
        m_binded_gv->xbool_invert_selected(logical_index);
        xgeom_tv->update();
      }
    case basic_types::type_double : {
        break;
      }
    case basic_types::type_float : case basic_types::type_real : {
        bool ok;
        double new_val =
            QInputDialog::getDouble(this,
                                    "Enter new value",
                                    QString::fromStdString(
                                      m_binded_gv->m_geom->field_name(logical_index)
                                      ),
                                    0, -10000, 10000, 4, &ok
                                    );
        if (ok) m_binded_gv->xfill<float>(logical_index, float(new_val));
        break;
      }
    case basic_types::type_int : {
        break;
      }
    case basic_types::type_string : {
        break;
      }
    }

  astate->make_viewport_dirty();

}
