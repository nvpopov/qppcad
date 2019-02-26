#include <qppcad/ws_item/geom_view/qmeasurements_table_model.hpp>

using namespace qpp;
using namespace qpp::cad;

int qatomic_dist_table_model_t::rowCount(const QModelIndex &parent) const {
  if (m_al) {
      return m_al->m_measure->m_dist_recs.size();
    } else return 0;
}

int qatomic_dist_table_model_t::columnCount(const QModelIndex &parent) const {
  return 7;
  //at1_name at1_id at1_idx at2_name at2_id at2_idx dist
}

QVariant qatomic_dist_table_model_t::data(const QModelIndex &index, int role) const {

  if (!m_al) return QVariant();

  if (role == Qt::DisplayRole) {

      switch (index.column()) {
        case 0 :
          return QString::fromStdString(
                m_al->m_geom->atom_name(m_al->m_measure->m_dist_recs[index.row()].m_at1));
          break;
        case 1 :
          return QString::fromStdString(
                fmt::format("{}",m_al->m_measure->m_dist_recs[index.row()].m_at1));
          break;
        case 2 :
          return QString::fromStdString(
                fmt::format("{}",m_al->m_measure->m_dist_recs[index.row()].m_idx1));
          break;
        case 3 :
          return QString::fromStdString(
                m_al->m_geom->atom_name(m_al->m_measure->m_dist_recs[index.row()].m_at2));
          break;
        case 4 :
          return QString::fromStdString(
                fmt::format("{}",m_al->m_measure->m_dist_recs[index.row()].m_at2));
          break;
        case 5 :
          return QString::fromStdString(
                fmt::format("{}",m_al->m_measure->m_dist_recs[index.row()].m_idx2));
        case 6 :
          return QString::fromStdString(
                fmt::format("{}",m_al->m_measure->dist(index.row())));
        default:
          return QVariant();
          break;
        }
    }

  return QVariant();
}

QVariant qatomic_dist_table_model_t::headerData(int section,
                                                Qt::Orientation orientation,
                                                int role) const {

  if (role == Qt::DisplayRole) {
      if (orientation == Qt::Horizontal) {
          switch (section) {
            case 0:
              return tr("At1");
              break;
            case 1:
              return tr("Id1");
              break;
            case 2:
              return tr("Idx1");
              break;
            case 3:
              return tr("At2");
              break;
            case 4:
              return tr("Id2");
              break;
            case 5:
              return tr("Idx2");
            case 6:
              return tr("R");
              break;
            default:
              return QString("");
            }
        }
    }

  return QVariant();

}

Qt::ItemFlags qatomic_dist_table_model_t::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = Qt::ItemFlag::ItemIsEnabled;
  return flags;
}

bool qatomic_dist_table_model_t::setData(const QModelIndex &index,
                                         const QVariant &value,
                                         int role) {
  return QAbstractTableModel::setData(index, value, role);
}

void qatomic_dist_table_model_t::bind(geom_view_t *_al) {
   m_al = _al;
}

void qatomic_dist_table_model_t::unbind() {
  m_al = nullptr;
}
