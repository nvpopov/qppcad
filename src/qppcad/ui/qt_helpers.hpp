#ifndef QPPCAD_QT_HELPERS
#define QPPCAD_QT_HELPERS

#include <QTableView>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QFormLayout>
#include <QLabel>

namespace qpp {

namespace cad {

class qt_hlp {

public:

  template<typename T>
  static void vrt_resize_tv_to_cnt(T *table_view) {
    int count = table_view->verticalHeader()->count();
    int horizontal_header_height = table_view->horizontalHeader()->height();
    int row_total_height = 0;
    for (int i = 0; i < count; ++i) {
      row_total_height += table_view->verticalHeader()->sectionSize(i);
    }
    table_view->setMinimumHeight(horizontal_header_height + row_total_height + 2);
    table_view->setMaximumHeight(horizontal_header_height + row_total_height + 2);
  }

  static void resize_form_lt_lbl(QLabel *label, int new_size);

  static void resize_form_lt_lbls(QFormLayout *form_lt, int new_size);

  static void form_lt_hide_row(QFormLayout *form_lt,
                               QLabel *field_label,
                               QWidget *field_widget);

  static void form_lt_insert_before(QFormLayout *form_lt,
                                    QWidget *target,
                                    QLabel *field_label,
                                    QWidget *field_widget);

  static void form_lt_ctrl_visibility(bool show,
                                      QFormLayout *form_lt,
                                      QWidget *target,
                                      QLabel *field_label,
                                      QWidget *field_widget);

  static QString clamp_string(const QString &src,
                              int max_width = 22,
                              const QString &suf = "...");

};

} // namespace qpp::cad

} // namespace qpp

#endif
