#include <qppcad/ws_atoms_list/qtype_specific_rendering_model.hpp>
#include <qppcad/app_state.hpp>

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
  return 2;
}

QVariant qtype_specific_rendering_model_t::data(const QModelIndex &index,
                                                int role) const {
  if (!m_al) return QVariant();

  if (role == Qt::DisplayRole) {

      switch (index.column()) {
        case 0 :
          return QString::fromStdString(m_al->m_geom->atom_of_type(index.row()));
          break;
        case 1 :
          return m_al->m_atom_type_to_hide.find(index.row()) != m_al->m_atom_type_to_hide.end();
          break;
        default:
          return QVariant();
          break;
        }
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
              return tr("Hide");
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
  return flags;
}

bool qtype_specific_rendering_model_t::setData(const QModelIndex &index,
                                               const QVariant &value,
                                               int role){
  app_state_t* astate = app_state_t::get_inst();

  if (!m_al) return false;
}

void qtype_specific_rendering_model_t::bind(ws_atoms_list_t *_al) {
  m_al = _al;
}

void qtype_specific_rendering_model_t::unbind() {
  //
}
