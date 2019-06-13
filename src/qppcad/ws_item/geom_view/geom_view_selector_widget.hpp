#ifndef QPPCAD_GEOM_VIEW_SELECTOR_WIDGET
#define QPPCAD_GEOM_VIEW_SELECTOR_WIDGET

#include <qppcad/qppcad.hpp>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>

namespace qpp {

  namespace cad {

    class qgeom_view_selector_widget_t : public QWidget {

        Q_OBJECT

      public:

        QHBoxLayout *main_lt;
        QListWidget *list_gv{nullptr};
        QListWidget *list_gv_res{nullptr};

        QVBoxLayout *add_btn_lt;
        QPushButton *add_btn;

        void generate_list_gv_items();

        qgeom_view_selector_widget_t(QWidget *parent = 0);

    };

  } // namespace cad

} // namespace qpp

#endif
