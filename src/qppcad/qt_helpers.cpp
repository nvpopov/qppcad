#include <qppcad/qt_helpers.hpp>
#include <QHeaderView>
#include <QScrollBar>

using namespace qpp;
using namespace qpp::cad;

void qt_helpers::vrt_resize_tv_to_content(QTableView *table_view) {

  int count = table_view->verticalHeader()->count();
//  int scroll_bar_height = table_view->horizontalScrollBar()->height();
  int horizontal_header_height = table_view->horizontalHeader()->height();
  int row_total_height = 0;
  for (int i = 0; i < count; ++i) {
      row_total_height += table_view->verticalHeader()->sectionSize(i);
    }
  table_view->setMinimumHeight(horizontal_header_height + row_total_height /*+ scroll_bar_height*/);

}
