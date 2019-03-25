#ifndef QPPCAD_WS_ITEM_EXTENDED_EDITOR
#define QPPCAD_WS_ITEM_EXTENDED_EDITOR

#include <qppcad/qppcad.hpp>
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

    class ws_item_extended_editor_t : public QWidget {

        Q_OBJECT

    public:

        ws_item_t *m_binded_item{nullptr};
        bool m_claim_full_space{false};
        virtual void bind_to_item(ws_item_t *_binding_item);
        virtual void unbind_item();
        virtual void update_from_ws_item();
        ws_item_extended_editor_t();

    };

  }

}

#endif
