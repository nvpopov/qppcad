#include <qppcad/flow/sflow.hpp>
#include <qppcad/qppcad.hpp>

using namespace qpp;
using namespace qpp::cad;

int main() {

  std::cout << "Heflow, world!" << std::endl;

  sflow_context_t *fc = new sflow_context_t;

  for (int i = 0; i <= 9; i++) {
      auto _nd = std::make_shared<sflow_node_t>();
      _nd->m_node_name = fmt::format("n{}", i);
      fc->add_node(_nd);
    }

  fc->connect_node(fc->m_nodes[0], fc->m_nodes[1]);
  fc->connect_node(fc->m_nodes[1], fc->m_nodes[2]);
  fc->connect_node(fc->m_nodes[2], fc->m_nodes[3]);

  fc->connect_node(fc->m_nodes[9], fc->m_nodes[4]);
  fc->connect_node(fc->m_nodes[4], fc->m_nodes[1]);

  fc->connect_node(fc->m_nodes[5], fc->m_nodes[6]);
  fc->connect_node(fc->m_nodes[6], fc->m_nodes[1]);

  fc->connect_node(fc->m_nodes[7], fc->m_nodes[2]);
  fc->connect_node(fc->m_nodes[8], fc->m_nodes[3]);

  fc->compile_flow();
  fc->execute();

  fmt::print(std::cout, "total connections = {}\n", fc->m_con_data.size());

  delete fc;

  return 0;

}
