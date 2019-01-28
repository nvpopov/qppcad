#ifndef QPP_CAD_QT_HELPERS
#define QPP_CAD_QT_HELPERS

#include <QTableView>

namespace qpp {

  namespace cad {

    class qt_helpers {
      public:
        static void vrt_resize_tv_to_content(QTableView *table_view);
    };

  }

}

#endif
