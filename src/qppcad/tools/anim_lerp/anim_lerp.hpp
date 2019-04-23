#ifndef QPPCAD_TOOL_ANIM_LERP
#define QPPCAD_TOOL_ANIM_LERP

#include <qppcad/qppcad.hpp>
#include <qppcad/ws_item/ws_item_behaviour_manager.hpp>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QApplication>

namespace qpp {

  namespace cad {

    class geom_view_t;

    class anim_lerp_tool_t : public ws_item_tool_t {

    public:

      void exec(ws_item_t *item, uint32_t _error_ctx) override;

    };

    class anim_lerp_widget_t : public QDialog {

    public:

      QVBoxLayout *dialog_layout;
      QGroupBox *gb_alerp_par;
      QFormLayout *gb_alerp_par_lt;

      QComboBox *alerp_anim1;
      QComboBox *alerp_anim2;
      QComboBox *alerp_anim1_frame;
      QComboBox *alerp_anim2_frame;

      QSpinBox *alerp_total_fr;

      QDialogButtonBox *dialog_bb;

      geom_view_t *m_gv{nullptr};

      anim_lerp_widget_t();

      void bind_geom_view(geom_view_t *_gv);
      void populate_anims_cmbs();
      void cmb_anim_changed_marshall(QComboBox *cmb, QComboBox *frm, int cmb_idx);

     public slots:

       void cmb_anim1_changed(int index);
       void cmb_anim2_changed(int index);

    };

  } // namespace cad

} // namespace qpp

#endif
