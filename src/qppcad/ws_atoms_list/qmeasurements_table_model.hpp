#ifndef QPP_CAD_QMEASUREMENTS_TABLE_MODEL
#define QPP_CAD_QMEASUREMENTS_TABLE_MODEL

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_atoms_list/ws_atoms_list.hpp>
#include <QAbstractTableModel>

namespace qpp {

  namespace cad {

    class qatomic_dist_table_model_t : public QAbstractTableModel {

        Q_OBJECT

      private:
         ws_atoms_list_t *m_al{nullptr};

      public:
        void bind(ws_atoms_list_t *_al);
        void unbind();

    };

  }

}


#endif

