#ifndef QPP_CAD_TOOL_STRUCTURE_SIMILARITY
#define QPP_CAD_TOOL_STRUCTURE_SIMILARITY
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <qppcad/ws_item/geom_view/geom_view.hpp>
#include <QTextEdit>
#include <QTableWidget>
#include <QHeaderView>

namespace qpp {

  namespace cad {

    struct structure_similarity_anim_info_t {

        QGroupBox *gb_itm_anim;
        QFormLayout *gb_itm_anim_lt;
        QComboBox *cmb_anim_name;
        QComboBox *cmb_anim_frame;
        void set_visible(bool visible);
        structure_similarity_anim_info_t(QLayout *lt, QString gb_title);

    };

    class structure_similarity_widget_t : public QDialog {

        Q_OBJECT

      private:

      public:

        QGroupBox *gb_select_actors;
        QFormLayout *gb_select_actors_lt;
        QHBoxLayout *widget_top_lt;
        QVBoxLayout *widget_lt;

        std::array<structure_similarity_anim_info_t*, 2> m_anim_info;

        QComboBox *cmb_ws1;
        QComboBox *cmb_it1;
        QComboBox *cmb_ws2;
        QComboBox *cmb_it2;
        QComboBox *cmb_method;
        QPushButton *btn_compute;

        QGroupBox *gb_str_sim_output;
        QVBoxLayout *gb_str_sim_output_lt;
        QTextEdit *str_sim_output;
        QTableWidget *str_sim_table;

        structure_similarity_widget_t();
        void cmb_ws(QComboBox *_cmb);
        void cmb_it(QComboBox *_cmb, std::shared_ptr<workspace_t> _ws, size_t itm_idx);
        void compute_structure_similarity(ws_item_t *g1, ws_item_t *g2);
        void compute_structure_similarity_naive(geom_view_t *g1, geom_view_t *g2);
        void compute_structure_similarity_tws_tree(geom_view_t *g1, geom_view_t *g2);
        void set_out_table_data(geom_view_t *g1,
                                geom_view_t *g2,
                                size_t atom_idx,
                                vector3<float> _dp);
        void cmb_it_changed_marshall(int index, size_t actor_id);
        void cmb_anim_changed_marshall(int index, size_t actor_id);

      public slots:

        void compute_button_clicked();
        void cmb_ws1_changed(int index);
        void cmb_ws2_changed(int index);
        void cmb_it1_changed(int index);
        void cmb_it2_changed(int index);
        void cmb_anim1_changed(int index);
        void cmb_anim2_changed(int index);

    };

    class structure_similarity_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item) override;

    };

  }

}

#endif
