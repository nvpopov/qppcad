#ifndef QPPCAD_SFLOW_BASE_NODES_VECTOR3F
#define QPPCAD_SFLOW_BASE_NODES_VECTOR3F

#include <qppcad/sflow/sflow_node.hpp>

namespace qpp {

namespace cad {

class sf_v3f_prop_node_t final : public sf_node_t {

public:

  sf_v3f_prop_node_t();
  bool execute_ex() override;
  bool is_single_node() override;

};

class sf_v3f_final_node_t final : public sf_node_t {

public:

  sf_v3f_final_node_t();
  bool execute_ex() override;
  bool is_single_node() override;

};

class sf_v3f_sum_v3f_node_t final : public sf_node_t {

public:

  sf_v3f_sum_v3f_node_t();
  bool execute_ex() override;

};

class sf_v3f_dot_node_t final : public sf_node_t {

public:

  sf_v3f_dot_node_t();
  bool execute_ex() override;

};

class sf_v3f_normalized_node_t final : public sf_node_t {

public:

  sf_v3f_normalized_node_t();
  bool execute_ex() override;

};

class sf_v3f_norm_node_t final : public sf_node_t {

public:

  sf_v3f_norm_node_t();
  bool execute_ex() override;

};

class sf_v3f_cross_node_t final : public sf_node_t {

public:

  sf_v3f_cross_node_t();
  bool execute_ex() override;

};

} // namespace cad

} // namespace qpp

#endif
