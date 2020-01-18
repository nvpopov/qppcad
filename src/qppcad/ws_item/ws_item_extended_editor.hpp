#ifndef QPPCAD_WS_ITEM_EXTENDED_EDITOR
#define QPPCAD_WS_ITEM_EXTENDED_EDITOR

#include <qppcad/core/qppcad.hpp>
#include <qppcad/ws_item/ws_item.hpp>
#include <qppcad/ui/qbinded_inputs.hpp>
#include <qppcad/ui/qspoiler_widget.hpp>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>

namespace qpp {

  namespace cad {

    enum ext_editor_geom_policy_e : int {

      always_max,
      always_min,
      start_max,
      start_min

    };

    class ws_item_extended_editor_t : public QWidget {

        Q_OBJECT

    public:

        ws_item_t *m_binded_item{nullptr};

        ws_item_extended_editor_t();

        virtual ext_editor_geom_policy_e get_geom_policy();

        virtual bool can_be_binded_to(ws_item_t* _item_to_bind);

        virtual int get_minimum_width();
        virtual void bind_to_item(ws_item_t *_binding_item);
        virtual void unbind_item();
        virtual void update_from_ws_item();
        virtual QString header_name_hint() = 0;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
