#include <qppcad/ws_item/geom_view/qtype_specific_rendering_model.hpp>
#include <qppcad/core/app_state.hpp>

using namespace qpp;
using namespace qpp::cad;

int qtype_specific_rendering_model_t::rowCount(const QModelIndex &parent) const {
  if (m_al) {
      return m_al->m_geom->n_atom_types();
    }
  else {
      return 0;
    }
}

int qtype_specific_rendering_model_t::columnCount(const QModelIndex &parent) const {
  return 3;
}

QVariant qtype_specific_rendering_model_t::data(const QModelIndex &index,
                                                int role) const {
  if (!m_al) return QVariant();

  if (role == Qt::DisplayRole) {

      switch (index.column()) {
        case 0 :
          return QString::fromStdString(m_al->m_geom->atom_of_type(index.row()));
          break;
        default:
          return QVariant();
          break;
        }
    }

  if(index.column() == 1 && role == Qt::CheckStateRole) {
      if (m_al->m_atom_type_to_hide.find(index.row()) !=
          m_al->m_atom_type_to_hide.end()) return Qt::Checked;
      else return Qt::Unchecked;
    }

  if(index.column() == 2 && role == Qt::CheckStateRole) {
      if (m_al->m_atom_type_to_hide_bond.find(index.row()) !=
          m_al->m_atom_type_to_hide_bond.end()) return Qt::Checked;
      else return Qt::Unchecked;
    }


  if (role == Qt::TextAlignmentRole) return Qt::AlignCenter;

  return QVariant();

}

QVariant qtype_specific_rendering_model_t::headerData(int section,
                                                      Qt::Orientation orientation,
                                                      int role) const {
  if (role == Qt::DisplayRole) {
      if (orientation == Qt::Horizontal) {
          switch (section) {
            case 0:
              return tr("Type");
              break;
            case 1:
              return tr("Hide Atom");
              break;
            case 2:
              return tr("Hide Bond");
              break;
            default:
              return QString("");
            }
        }
    }

  return QVariant();

}

Qt::ItemFlags qtype_specific_rendering_model_t::flags(const QModelIndex &index) const {

  Qt::ItemFlags flags = Qt::ItemFlag::ItemIsEnabled;

  if (index.column() == 1 || index.column() == 2) {
      flags = flags | Qt::ItemFlag::ItemIsUserCheckable ;
    }

  return flags;
}

bool qtype_specific_rendering_model_t::setData(const QModelIndex &index,
                                               const QVariant &value,
                                               int role){
  app_state_t* astate = app_state_t::get_inst();

  if (!m_al) return false;

  if (index.column() == 1 && role == Qt::CheckStateRole) {
      if (value == Qt::Checked) {
          m_al->m_atom_type_to_hide.insert(index.row());
        } else {
          auto it = m_al->m_atom_type_to_hide.find(index.row());
          if (it != m_al->m_atom_type_to_hide.end()) m_al->m_atom_type_to_hide.erase(it);
        }
      astate->make_viewport_dirty();
      return true;
    }

  if (index.column() == 2 && role == Qt::CheckStateRole) {
      if (value == Qt::Checked) {
          m_al->m_atom_type_to_hide_bond.insert(index.row());
        } else {
          auto it = m_al->m_atom_type_to_hide_bond.find(index.row());
          if (it != m_al->m_atom_type_to_hide_bond.end()) m_al->m_atom_type_to_hide_bond.erase(it);
        }
      astate->make_viewport_dirty();
      return true;
    }

  return QAbstractTableModel::setData(index, value, role);

}

void qtype_specific_rendering_model_t::bind(geom_view_t *_al) {

  m_al = _al;

}

void qtype_specific_rendering_model_t::unbind() {

  m_al = nullptr;

}
