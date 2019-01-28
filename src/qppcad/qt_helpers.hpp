#ifndef QPP_CAD_QT_HELPERS
#define QPP_CAD_QT_HELPERS

#include <QTableView>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
namespace qpp {

  namespace cad {

    class qt_helpers {
      public:

        template<typename T>
        static void vrt_resize_tv_to_content(T *table_view) {
          int count = table_view->verticalHeader()->count();
          int horizontal_header_height = table_view->horizontalHeader()->height();
          int row_total_height = 0;
          for (int i = 0; i < count; ++i) {
              row_total_height += table_view->verticalHeader()->sectionSize(i);
            }
          table_view->setMinimumHeight(horizontal_header_height + row_total_height + 2);
        }

    };

  }

}

#endif
