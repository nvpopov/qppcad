#ifndef QPPCAD_CCD_VIB_ANALYZER_EXTENDED_EDITOR
#define QPPCAD_CCD_VIB_ANALYZER_EXTENDED_EDITOR
#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/ccd_view/ccd_view.hpp>

namespace qpp {

  namespace cad {

    class ccd_vib_analyzer_extended_editor_t : public ws_item_extended_editor_t {

        Q_OBJECT

      public:

        ccd_view_t *m_binded_nx{nullptr};
        QVBoxLayout *main_lt;

        ccd_vib_analyzer_extended_editor_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        bool can_be_binded_to(ws_item_t *item) override;

        QString header_name_hint() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
