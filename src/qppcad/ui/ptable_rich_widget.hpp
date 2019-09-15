#ifndef QPPCAD_PTABLE_RICH_WIDGET_H
#define QPPCAD_PTABLE_RICH_WIDGET_H

#include <qppcad/core/qppcad.hpp>
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class ptable_rich_widget_t : public QDialog {

        Q_OBJECT

      public:

        QHBoxLayout *m_main_lt;
        QGridLayout *m_elem_lt;

        explicit ptable_rich_widget_t(QWidget *parent = nullptr);
        void construct_widgets();

        std::optional<std::tuple<int, int> > get_ptable_x_y(size_t elem_id);

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
