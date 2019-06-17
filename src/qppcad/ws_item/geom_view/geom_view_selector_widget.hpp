#ifndef QPPCAD_GEOM_VIEW_SELECTOR_WIDGET
#define QPPCAD_GEOM_VIEW_SELECTOR_WIDGET

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>

namespace qpp {

  namespace cad {

    struct geom_view_selection_query_t {

        geom_view_t *gv{nullptr};
        int anim_id{-1};
        int frame_id{-1};

    };

    class qgeom_view_selector_entry_t : public QFrame {

      Q_OBJECT

      public:

        geom_view_t *binded_gv{nullptr};

        size_t stored_ws_idx{0};
        size_t stored_ws_itm_idx{0};

        QVBoxLayout *main_lt;
        QHBoxLayout *ctrls_lt;
        QLabel *gv_name;
        QComboBox *gv_anim_name;
        QComboBox *gv_frame_id;
        QPushButton *up_btn;
        QPushButton *down_btn;

        explicit qgeom_view_selector_entry_t(QWidget *parent = nullptr);
        //void delete_record();
        void rebuild();

      public slots:

        void cmb_anim_name_changed(int idx);

    };

    class qgeom_view_result_widget_t : public QWidget {

        Q_OBJECT

      public:

        QVBoxLayout *main_lt;

        explicit qgeom_view_result_widget_t(QWidget *parent = nullptr);

    };

    class qgeom_view_selector_widget_t : public QWidget {

        Q_OBJECT

      public:

        QHBoxLayout *main_lt;
        QVBoxLayout *list_gv_lbl_lt;
        QVBoxLayout *list_gv_res_lbl_lt;

        QListWidget *list_gv{nullptr};
        QScrollArea *list_gv_res_scroll_area;
        qgeom_view_result_widget_t *list_gv_res{nullptr};

        QLabel *list_gv_lbl;
        QLabel *list_gv_res_lbl;

        QVBoxLayout *add_btn_lt;
        QPushButton *add_btn;

        std::vector<geom_view_t*> m_sub_gvs;

        void generate_list_gv_items();
        void compose_selection_query(std::vector<geom_view_selection_query_t> &sel_query);
        qgeom_view_selector_widget_t(QWidget *parent = 0);

      public slots:

        void add_btn_clicked();

    };

  } // namespace cad

} // namespace qpp

#endif
