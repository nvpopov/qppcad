#include <qppcad/qbonding_table_model.hpp>
#include <iterator>

using namespace qpp;
using namespace qpp::cad;

int qbonding_table_model_t::rowCount(const QModelIndex &parent) const {

  if (m_al) {
      return m_al->m_tws_tr->m_bonding_table.m_dist.size();
    }
  else {
      return 0;
    }

}

int qbonding_table_model_t::columnCount(const QModelIndex &parent) const {
  return 4;
}

QVariant qbonding_table_model_t::data(const QModelIndex &index, int role) const {

  if (role == Qt::DisplayRole) {

      auto brec = m_al->m_tws_tr->m_bonding_table.m_dist.begin();
      std::advance(brec, index.row());

      if (brec != m_al->m_tws_tr->m_bonding_table.m_dist.end()) {
          switch (index.column()) {
            case 0 :
              return QString::fromStdString(m_al->m_geom->atom_of_type(brec->first.m_a));
              break;
            case 1 :
              return QString::fromStdString(m_al->m_geom->atom_of_type(brec->first.m_b));
              break;
            case 2 :
              return QVariant(brec->second.m_bonding_dist);
              break;
            case 3 :
              return QVariant(brec->second.m_enabled);
              break;
            default:
              break;
            }
        }
    }

  return QVariant();

}

QVariant qbonding_table_model_t::headerData(int section,
                                            Qt::Orientation orientation,
                                            int role) const {

  if (role == Qt::DisplayRole) {
      if (orientation == Qt::Horizontal) {
          switch (section) {
            case 0:
              return tr("First");
              break;
            case 1:
              return tr("Second");
              break;
            case 2:
              return tr("Distance");
              break;
            case 3:
              return tr("Enabled");
              break;
            default:
              return QString("");
            }
        }
    }

  return QVariant();

}

void qbonding_table_model_t::bind(ws_atoms_list_t *_al) {
  m_al = _al;
}

void qbonding_table_model_t::unbind() {

}
