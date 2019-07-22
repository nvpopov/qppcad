#ifndef QPPCAD_NODE_BOOK_EXTENDED_EDITOR
#define QPPCAD_NODE_BOOK_EXTENDED_EDITOR

#include <qppcad/ws_item/ws_item_extended_editor.hpp>
#include <qppcad/ws_item/node_book/node_book.hpp>
#include <qppcad/ws_item/node_book/node_book_graphics_view.hpp>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace qpp {

  namespace cad {

    class node_book_extended_editor_t : public ws_item_extended_editor_t {

        Q_OBJECT

      public:

        node_book_t *m_binded_nb{nullptr};
        node_book_graphics_view_t *m_gr_view;
        QVBoxLayout *main_lt;
        QRectF m_stored_scene_rect;
        bool m_binded_first_time{true};

        node_book_extended_editor_t();

        void bind_to_item(ws_item_t *_binding_item) override;
        void update_from_ws_item() override;
        void unbind_item() override;

        bool can_be_binded_to(ws_item_t *item) override;

        QString header_name_hint() override;

    };

  } // namespace qpp::cad

} // namespace qpp

#endif
