#ifndef QPP_CAD_TYPE_SPEC_RENDERING_MODEL
#define QPP_CAD_TYPE_SPEC_RENDERING_MODEL

#include <qppcad/qppcad.hpp>
#include <qppcad/geom_view/geom_view.hpp>
#include <QAbstractTableModel>

namespace qpp {

  namespace cad {

    class qtype_specific_rendering_model_t : public QAbstractTableModel {

        Q_OBJECT

      private:
        geom_view_t *m_al{nullptr};
        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

      public:
        void bind(geom_view_t *_al);
        void unbind();
    };

  }

}


#endif
