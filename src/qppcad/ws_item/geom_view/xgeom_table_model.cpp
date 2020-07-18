#include <qppcad/ws_item/geom_view/xgeom_table_model.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;


int xgeom_table_model_t::rowCount(const QModelIndex &parent) const {

  if (!m_gv || !m_gv->m_geom) return 0;
  return m_gv->m_geom->nat();

}

int xgeom_table_model_t::columnCount(const QModelIndex &parent) const {

  if (!m_gv || !m_gv->m_geom) return 0;
  return m_gv->m_geom->nfields();

}

QVariant xgeom_table_model_t::data(const QModelIndex &index, int role) const {

  if (!m_gv || !m_gv->m_geom) return QVariant();

  if (!index.isValid()) return QVariant();

  if (role == Qt::TextAlignmentRole) {
      return Qt::AlignCenter;
    }

  int xfield_index = index.column();
  int atom_id = index.row();
  basic_types field_type = m_gv->m_geom->field_type(xfield_index);

  if (role == Qt::BackgroundRole
      && m_gv->m_atom_idx_sel.find({atom_id, index::D(m_gv->m_geom->get_DIM()).all(0)}) !=
         m_gv->m_atom_idx_sel.end()) {

      return QVariant(QColor(Qt::gray));
    }

  if (role == Qt::DisplayRole || role == Qt::EditRole) {

     switch (field_type) {

       case basic_types::type_bool : {
           return QVariant();
         }

       case basic_types::type_double : {
           double field_val = m_gv->m_geom->xfield<double>(xfield_index, atom_id);
           return QVariant(field_val);
         }

       case basic_types::type_float : {
           float field_val = m_gv->m_geom->xfield<float>(xfield_index, atom_id);
           return QVariant(field_val);
         }

       case basic_types::type_real : {
           float field_val = m_gv->m_geom->xfield<float>(xfield_index, atom_id);
           return QVariant(field_val);
         }

       case basic_types::type_int : {
           return QString("%1").arg(m_gv->m_geom->xfield<int>(xfield_index, atom_id));
         }

       case basic_types::type_string : {
           QString field_val = QString::fromStdString(
                              m_gv->m_geom->xfield<std::string>(xfield_index, atom_id));
           return field_val;
         }

       }

     return QVariant();

    }

  if (field_type == basic_types::type_bool && role == Qt::CheckStateRole) {
      if (m_gv->m_geom->xfield<bool>(xfield_index, atom_id)) return Qt::Checked;
      else return Qt::Unchecked;
    }

  return QVariant();

}

QVariant xgeom_table_model_t::headerData(int section,
                                         Qt::Orientation orientation, int role) const {

  if (!m_gv || !m_gv->m_geom) return QVariant();

  if (role == Qt::DisplayRole) {
      if (orientation == Qt::Horizontal) {
          std::string field_name = m_gv->m_geom->field_name(section);
          return QVariant(QString::fromStdString(field_name));
        }
      if (orientation == Qt::Vertical) {
          return section;
        };
    }

  return QVariant();

}

Qt::ItemFlags xgeom_table_model_t::flags(const QModelIndex &index) const {

  Qt::ItemFlags flags =  QAbstractItemModel::flags(index) |
                         Qt::ItemFlag::ItemIsEnabled |
                         Qt::ItemFlag::ItemIsEditable;

  if (!m_gv || !m_gv->m_geom) return flags;

  if (!index.isValid())
        return Qt::ItemIsEnabled;

  if (m_gv->m_geom) {
      int xfield_index = index.column();
      int atom_id = index.row();
      basic_types field_type = m_gv->m_geom->field_type(xfield_index);
      if (field_type == basic_types::type_bool)
        flags = flags | Qt::ItemFlag::ItemIsUserCheckable;
    }

  return flags;

}

bool xgeom_table_model_t::setData(const QModelIndex &index, const QVariant &value, int role) {

  app_state_t *astate = app_state_t::get_inst();

  if (m_gv->m_geom) {

      int xfield_index = index.column();
      int atom_id = index.row();
      basic_types field_type = m_gv->m_geom->field_type(xfield_index);

      if (role == Qt::CheckStateRole && field_type == basic_types::type_bool) {
          m_gv->m_geom->xfield<bool>(xfield_index, atom_id) = value == Qt::Checked;
          astate->make_viewport_dirty();
          return true;
        }

      if (role == Qt::EditRole)
        switch (field_type) {
          case basic_types::type_bool : {
              return QAbstractTableModel::setData(index, value, role);
              break;
            }
          case basic_types::type_double : {
              m_gv->m_geom->xfield<double>(xfield_index, atom_id) = value.toDouble();
              astate->make_viewport_dirty();
              return true;
              break;
            }
          case basic_types::type_float : {
              m_gv->m_geom->xfield<float>(xfield_index, atom_id) = value.toDouble();
              astate->make_viewport_dirty();
              return true;
              break;
            }
          case basic_types::type_real : {
              m_gv->m_geom->xfield<float>(xfield_index, atom_id) = value.toDouble();
              astate->make_viewport_dirty();
              return true;
              break;
            }
          case basic_types::type_int : {
              m_gv->m_geom->xfield<int>(xfield_index, atom_id) = value.toInt();
              astate->make_viewport_dirty();
              return true;
              break;
            }
          case basic_types::type_string : {
              std::string new_val = value.toString().toStdString();
              if (m_gv->m_geom->field_name(xfield_index) == "atom") {
                  m_gv->m_geom->change(atom_id, new_val, m_gv->m_geom->pos(atom_id));
                } else {
                  m_gv->m_geom->xfield<std::string>(xfield_index, atom_id) = new_val;
                }
              astate->make_viewport_dirty();
              return true;
              break;
            }
          }

    }

  return QAbstractTableModel::setData(index, value, role);

}

void xgeom_table_model_t::bind(geom_view_t *_gv) {

  m_gv = _gv;

}

void xgeom_table_model_t::unbind() {

  m_gv = nullptr;

}
