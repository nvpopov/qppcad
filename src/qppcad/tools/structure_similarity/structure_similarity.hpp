#ifndef QPP_CAD_TOOL_STRUCTURE_SIMILARITY
#define QPP_CAD_TOOL_STRUCTURE_SIMILARITY
#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>

namespace qpp {

  namespace cad {

    class structure_similarity_widget_t : public QDialog {

        Q_OBJECT

      private:

      public:

        QGroupBox *gb_select_actors;
        QFormLayout *gb_select_actors_lt;
        QVBoxLayout *widget_layout;

        QComboBox *cmb_ws1;
        QComboBox *cmb_it1;
        QComboBox *cmb_ws2;
        QComboBox *cmb_it2;

        QGroupBox *gb_str_sim_output;
        QVBoxLayout *gb_str_sim_output_lt;
        structure_similarity_widget_t();
        void cmb_ws(QComboBox *_cmb);
        void cmb_it(QComboBox *_cmb, std::shared_ptr<workspace_t> _ws);

      public slots:

        void cmb_ws1_changed(int index);
        void cmb_ws2_changed(int index);

    };

    class structure_similarity_tool_t : public ws_item_tool_t {

      public:

        void exec(ws_item_t *item) override;

    };

  }

}

#endif
