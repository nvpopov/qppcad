#ifndef QPP_CAD_REAL_SPINBOX_DELEGATE
#define QPP_CAD_REAL_SPINBOX_DELEGATE

#include <qppcad/qppcad.hpp>
#include <QStyledItemDelegate>
#include <QDoubleSpinBox>

namespace qpp {

  namespace cad {

    class qrealspinbox_delegate_t : public QStyledItemDelegate {

        Q_OBJECT

      public:

        double m_spinbox_min{0.01};
        double m_spinbox_max{5.0};
        double m_spinbox_step{0.01};

        qrealspinbox_delegate_t(QObject *parent = 0);

        void set_min_max_step(double _min,
                              double _max,
                              double _step);

        QWidget *createEditor(QWidget *parent,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

        void setEditorData(QWidget *editor,
                           const QModelIndex &index) const override;

        void setModelData(QWidget *editor,
                          QAbstractItemModel *model,
                          const QModelIndex &index) const override;

        void updateEditorGeometry(QWidget *editor,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &) const override;
    };

  }

}

#endif
