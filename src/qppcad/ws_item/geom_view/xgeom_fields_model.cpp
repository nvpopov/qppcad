#include <qppcad/ws_item/geom_view/xgeom_fields_model.hpp>

using namespace qpp;
using namespace qpp::cad;

int qxgeom_fields_model_t::rowCount(const QModelIndex &parent) const {

  if (m_al) return m_al->m_geom->nfields();
  else return 0;

}

int qxgeom_fields_model_t::columnCount(const QModelIndex &parent) const {

  return 4;

}

QVariant qxgeom_fields_model_t::data(const QModelIndex &index, int role) const {

  if (!m_al) return QVariant();

  if (role == Qt::TextAlignmentRole) {
      return Qt::AlignCenter;
    }

  if (role == Qt::DisplayRole) {

      auto conv_basic_types = [](basic_types btype) {

        switch (btype) {

          case basic_types::type_int :
            return "int";
            break;
          case basic_types::type_bool :
            return "bool";
            break;
          case basic_types::type_float :
            return "float";
            break;
          case basic_types::type_real :
            return "real";
            break;
          case basic_types::type_double :
            return "double";
            break;
          case basic_types::type_string :
            return "string";
            break;

          }

        return "";

      };

      switch (index.column()) {

        case 0 :
          return index.row();
          break;

        case 1 :
          return QString::fromStdString(m_al->m_geom->field_name(index.row()));
          break;

        case 2 :
          return conv_basic_types(m_al->m_geom->field_type(index.row()));
          break;

        case 3 :
          return QVariant();
          break;

        default:
          return QVariant();
          break;

        }

    }

  if (index.column() == 3 && role == Qt::CheckStateRole) {
      return index.row() <= 5 ? Qt::Checked : Qt::Unchecked;
    }

  return QVariant();

}

QVariant qxgeom_fields_model_t::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const {

  if (role == Qt::DisplayRole) {

      if (orientation == Qt::Horizontal) {
          switch (section) {

            case 0:
              return tr("Index");
              break;

            case 1:
              return tr("Name");
              break;

            case 2:
              return tr("Type");
              break;

            case 3:
              return tr("Persistent");
              break;

            default:
              return QString("");

            }

        }

    }

  return QVariant();

}

Qt::ItemFlags qxgeom_fields_model_t::flags(const QModelIndex &index) const {

  Qt::ItemFlags flags = Qt::ItemFlag::ItemIsEnabled;
  return flags;

}

bool qxgeom_fields_model_t::setData(const QModelIndex &index, const QVariant &value, int role) {
  return false;
}

void qxgeom_fields_model_t::bind(geom_view_t *_al) {

  m_al = _al;

}

void qxgeom_fields_model_t::unbind() {

  m_al = nullptr;

}
