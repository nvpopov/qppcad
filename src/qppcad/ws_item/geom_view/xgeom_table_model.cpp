#include <qppcad/ws_item/geom_view/xgeom_table_model.hpp>

using namespace qpp;
using namespace qpp::cad;


int xgeom_table_model_t::rowCount(const QModelIndex &parent) const {

}

int xgeom_table_model_t::columnCount(const QModelIndex &parent) const {

}

QVariant xgeom_table_model_t::data(const QModelIndex &index, int role) const {

}

QVariant xgeom_table_model_t::headerData(int section,
                                         Qt::Orientation orientation, int role) const {

}

Qt::ItemFlags xgeom_table_model_t::flags(const QModelIndex &index) const {

}

bool xgeom_table_model_t::setData(const QModelIndex &index, const QVariant &value, int role) {

}

void xgeom_table_model_t::bind(xgeometry<float, periodic_cell<float> > *_xgeom) {

}

void xgeom_table_model_t::unbind() {

}
