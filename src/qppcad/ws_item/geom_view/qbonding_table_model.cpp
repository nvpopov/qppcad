#include <qppcad/ws_item/geom_view/qbonding_table_model.hpp>
#include <qppcad/app_state.hpp>
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

  if (!m_al) return QVariant();

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
              return QVariant();
              break;
            }
        }
    }

  if(index.column() == 3 && role == Qt::CheckStateRole) {
      auto brec = m_al->m_tws_tr->m_bonding_table.m_dist.begin();
      std::advance(brec, index.row());
      if (brec->second.m_enabled) return Qt::Checked;
      else return Qt::Unchecked;
    }

  if (role == Qt::TextAlignmentRole) {
      if (index.column() >= 0 && index.column() <= 3) return Qt::AlignCenter;
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

Qt::ItemFlags qbonding_table_model_t::flags(const QModelIndex &index) const {

  Qt::ItemFlags flags = Qt::ItemFlag::ItemIsEnabled;

  if (index.column() == 0 || index.column() == 1) {
      flags = flags | Qt::ItemFlag::ItemIsSelectable;
    }

  if (index.column() == 2) {
      flags = flags | Qt::ItemFlag::ItemIsEditable;
    }

  if (index.column() == 3) {
      flags = flags | Qt::ItemFlag::ItemIsUserCheckable ;
    }

  return flags;

}

bool qbonding_table_model_t::setData(const QModelIndex &index, const QVariant &value, int role) {

  app_state_t* astate = app_state_t::get_inst();

  if (!m_al) return false;

  if (index.column() == 3 && role == Qt::CheckStateRole) {
      auto brec = m_al->m_tws_tr->m_bonding_table.m_dist.begin();
      std::advance(brec, index.row());
      brec->second.m_enabled = value == Qt::Checked;

      m_al->m_tws_tr->do_action(act_rebuild_ntable);
      astate->make_viewport_dirty();
      return true;
    }

  if (index.column() == 2 && role == Qt::EditRole) {
      auto brec = m_al->m_tws_tr->m_bonding_table.m_dist.begin();
      std::advance(brec, index.row());
      if (value.type() == QVariant::Double) {
          brec->second.m_bonding_dist = float(value.toDouble());
          m_al->m_tws_tr->m_bonding_table.update_pair_max_dist(brec->first.m_a, brec->first.m_b);
          m_al->m_tws_tr->do_action(act_rebuild_ntable);
          astate->make_viewport_dirty();
          return true;
       }
    }

  return QAbstractTableModel::setData(index, value, role);

}

void qbonding_table_model_t::bind(geom_view_t *_al) {
  m_al = _al;
}

void qbonding_table_model_t::unbind() {
  //
}
