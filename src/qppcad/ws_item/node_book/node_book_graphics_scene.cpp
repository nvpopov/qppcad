#include <qppcad/ws_item/node_book/node_book_graphics_scene.hpp>

using namespace qpp;
using namespace qpp::cad;


node_book_graphics_scene_t::node_book_graphics_scene_t(QObject *parent)
  : QGraphicsScene(parent) , p_clr_bckgr(QColor("#393939")) , p_clr_light(QColor("#2F2F2F")),
    p_clr_dark(QColor("#292929")), p_pen_light(QPen(p_clr_light)), p_pen_dark(QPen(p_clr_dark)),
    p_brush_bckgr(p_clr_bckgr) {

  for (auto p : {&p_pen_light, &p_pen_dark}) p->setWidth(0);
  setBackgroundBrush(p_brush_bckgr);

}

void node_book_graphics_scene_t::drawBackground(QPainter *painter, const QRectF &rect) {

  QGraphicsScene::drawBackground(painter, rect);

  const int gridsize = 20;
  int left = static_cast<int>(std::floor(rect.left()));
  int right = static_cast<int>(std::ceil(rect.right()));
  int top = static_cast<int>(std::floor(rect.top()));
  int bottom = static_cast<int>(std::ceil(rect.bottom()));

  const int first_left = left - (left % gridsize);
  const int first_top = top - (top % gridsize);

  std::vector<QLine> lines_light;
  std::vector<QLine> lines_dark;

  for (int x = first_left; x <= right; x += gridsize) {
      if (x % 100 != 0) lines_light.push_back(QLine(x, top, x, bottom));
      else lines_dark.push_back(QLine(x, top, x, bottom));
    }

  for (int y = first_top; y <= bottom; y += gridsize) {
      if (y % 100 != 0) lines_light.push_back(QLine(left, y, right, y));
      else lines_dark.push_back(QLine(left, y, right, y));
    }

  painter->setPen(p_pen_light);
  painter->drawLines(lines_light.data(), lines_light.size());

  painter->setPen(p_pen_dark);
  painter->drawLines(lines_dark.data(), lines_dark.size());

}
