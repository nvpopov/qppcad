#ifndef QPPCAD_SFLOW_STATUS
#define QPPCAD_SFLOW_STATUS

namespace qpp {

    namespace cad {

        enum sflow_status_e : int {
            no_error,
            pin1_invalid,
            pin2_invalid,
            error_nodes_are_equal,
            types_pin1_not_eq_pin2,
            too_many_inputs,
            propagate_succes,
            propagate_error
        };

    } //namespace cad

} // namespace qpp

#endif
