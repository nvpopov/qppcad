#ifndef QPPCAD_SFLOW_PARAMETER
#define QPPCAD_SFLOW_PARAMETER

#include <qppcad/core/qppcad.hpp>
#include <geom/xgeom.hpp>
#include <qppcad/core/qpp_object.hpp>
#include <qppcad/ws_item/ws_item.hpp>

namespace qpp {

  namespace cad {

    enum sflow_parameter_e : int {
      sfpar_none,
      sfpar_int,
      sfpar_bool,
      sfpar_float,
      sfpar_v3f,
      sfpar_string,
      sfpar_xgeom,
      sfpar_ws_item
    };

    class sflow_parameter_t : public qpp_object_t {

        QPP_OBJECT(sflow_parameter_t, qpp_object_t)

      public:

        virtual sflow_parameter_e get_param_meta();
        virtual std::shared_ptr<sflow_parameter_t> clone() = 0;
        explicit sflow_parameter_t();

    };

    class sflow_parameter_int_t : public sflow_parameter_t {

        QPP_OBJECT(sflow_parameter_int_t, sflow_parameter_t)

      public:

        int m_value;
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;

    };

    class sflow_parameter_v3f_t : public sflow_parameter_t {

        QPP_OBJECT(sflow_parameter_v3f_t, sflow_parameter_t)

      public:

        vector3<float> m_value;
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;

    };

    class sflow_parameter_bool_t : public sflow_parameter_t {

        QPP_OBJECT(sflow_parameter_bool_t, sflow_parameter_t)

      public:

        bool m_value;
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;

    };

    class sflow_parameter_float_t : public sflow_parameter_t {

        QPP_OBJECT(sflow_parameter_float_t, sflow_parameter_t)

      public:

        float m_value;
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;

    };

    class sflow_parameter_xgeom_t : public sflow_parameter_t {

        QPP_OBJECT(sflow_parameter_xgeom_t, sflow_parameter_t)

      public:

        xgeometry<float, periodic_cell<float> > m_value{3};
        sflow_parameter_e get_param_meta() override ;
        std::shared_ptr<sflow_parameter_t> clone() override;

    };

  } // namespace cad

} // namespace qpp

#endif
