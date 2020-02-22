#ifndef QPPCAD_SFLOW_STATUS
#define QPPCAD_SFLOW_STATUS

namespace qpp {

  namespace cad {

    enum sf_status_e : int {
      no_error,
      pin1_invalid,
      pin2_invalid,
      error_nodes_are_equal,
      types_pin1_not_eq_pin2,
      too_many_inputs,
      propagate_data_succes,
      propagate_data_error,
      propagate_meta_succes,
      propagate_meta_error
    };

  } //namespace cad

} // namespace qpp

#endif
