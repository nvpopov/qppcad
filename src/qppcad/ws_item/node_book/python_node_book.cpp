#include <qppcad/ws_item/node_book/python_node_book.hpp>

using namespace qpp;
using namespace qpp::cad;

void py_node_book_reg_helper_t::reg(
    pybind11::module &module,
    py::class_<ws_item_t, std::shared_ptr<ws_item_t>, py_ws_item_t> &ws_item_base) {

  py::class_<node_book_t, std::shared_ptr<node_book_t> >
  py_node_book_t(module, "node_book_t", ws_item_base);

  py_node_book_t.def("num_nds", [](node_book_t &src){return src.m_scene->m_nodes.size();});
  py_node_book_t.def("num_sck", [](node_book_t &src){return src.m_scene->m_sockets.size();});
  py_node_book_t.def("num_con", [](node_book_t &src){return src.m_scene->m_connections.size();});
  py_node_book_t.def("execute", &node_book_t::execute);

  py_node_book_t.def("get_vis_rect", &node_book_t::py_get_visible_rect);

}
